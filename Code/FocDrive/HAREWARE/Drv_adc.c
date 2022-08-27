#include "Drv_adc.h"
#include "foc.h"
#include "usart.h"

extern feedback_t fb;


__IO uint16_t AdcValue[2];

static void initAdcDMA(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);/*DMA2的时钟使能*/
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);/*等待DMA可以配置*/


	DMA_InitStructure.DMA_Channel = DMA_Channel_0;/*DMA通道0*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);//(uint32_t)(ADC1->DR);/*外设地址*/ //(uint32_t)(ADC1_BASE+0x4C);//
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&AdcValue;/*存取器地址*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;/*方向从外设到内存*/
	DMA_InitStructure.DMA_BufferSize = 2;/*数据传输的数量为1*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*地址不增加*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*地址增加*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*数据长度半字*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*数据长度半字*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*高优先级*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*循环模式*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;/*禁止FIFO*/
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO的值*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*单次传输*/
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*单次传输*/
	
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);/**/
	DMA_Cmd(DMA2_Stream0,ENABLE);//开启DMA传输  
}

void Drv_AdcInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能ADC时钟
	
	ADC_DeInit();
	/*初始化ADC1通道1 的IO口*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;/*模拟输入*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 |GPIO_Pin_1 ;/*通道0*/
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;/*不带上下拉*/
	GPIO_Init(GPIOA,&GPIO_InitStructure);/*初始化*/
	/*通用控制寄存器的配置*/
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMA失能*/
	ADC_CommonInitStructure.ADC_Mode          = ADC_Mode_Independent;/*独立模式*/
	ADC_CommonInitStructure.ADC_Prescaler     = ADC_Prescaler_Div4;/*APB2的4分频 即84/4=21M*/
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;/*两个采样阶段的延时5个时钟*/
	ADC_CommonInit(&ADC_CommonInitStructure);/*初始化*/
	/*初始化ADC1*/
	ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;/*12位模式*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;/*扫描模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;/*连续转换*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测 使用软件触发*/
	ADC_InitStructure.ADC_DataAlign    = ADC_DataAlign_Right;/*右对齐*/
	ADC_InitStructure.ADC_ExternalTrigConv = 0;
	ADC_InitStructure.ADC_NbrOfConversion = 2;/*2通道 1*/
	
	ADC_Init(ADC1,&ADC_InitStructure);/*初始化*/
	
	initAdcDMA();

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_15Cycles);/*设置规则通道3 二个序列 采样时间 */
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_15Cycles);/*设置规则通道3 二个序列 采样时间 */
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//打开ADC中断
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);//源数据变化时开启DMA传输
	ADC_Cmd(ADC1,ENABLE);/*开启转换*/
	
	
	
	ADC_DMACmd(ADC1,ENABLE);//使能ADC传输
	
	
	nvic.NVIC_IRQChannel = ADC_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic);

}

int calibration_ch0=0,calibration_ch1=0;

void ADC_IRQHandler()
{
	static u16 count=0;
	float temp_u,temp_v;
	if (count <= 300)
	{
		calibration_ch0 += (AdcValue[0]-2048);
		calibration_ch1 += (AdcValue[1]-2048);
		count ++;
	}
	else
	{
		temp_u = -(AdcValue[0]-2048-calibration_ch0/300)*3.3/4096;
		temp_v = (AdcValue[1]-2048-calibration_ch1/300)*3.3/4096;
		
		fb.current_u = 0.6 * temp_u + (0.4) * fb.current_u; 
		fb.current_v = 0.6 * temp_v + (0.4) * fb.current_v;
		
	//	printf("%d\r\n",AdcValue[1]-2048);
		
	//	fb.current_u = -((AdcValue[0]-2048)*3.3/4096);
	//	fb.current_v = (AdcValue[1]-2048)*3.3/4096;
		fb.current_w = -fb.current_u - fb.current_v;
	}
	
}

