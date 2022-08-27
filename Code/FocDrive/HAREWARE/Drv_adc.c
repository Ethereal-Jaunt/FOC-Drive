#include "Drv_adc.h"
#include "foc.h"
#include "usart.h"

extern feedback_t fb;


__IO uint16_t AdcValue[2];

static void initAdcDMA(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);/*DMA2��ʱ��ʹ��*/
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);/*�ȴ�DMA��������*/


	DMA_InitStructure.DMA_Channel = DMA_Channel_0;/*DMAͨ��0*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);//(uint32_t)(ADC1->DR);/*�����ַ*/ //(uint32_t)(ADC1_BASE+0x4C);//
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)&AdcValue;/*��ȡ����ַ*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;/*��������赽�ڴ�*/
	DMA_InitStructure.DMA_BufferSize = 2;/*���ݴ��������Ϊ1*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*��ַ������*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*��ַ����*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*���ݳ��Ȱ���*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*���ݳ��Ȱ���*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*�����ȼ�*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*ѭ��ģʽ*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;/*��ֹFIFO*/
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO��ֵ*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*���δ���*/
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*���δ���*/
	
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);/**/
	DMA_Cmd(DMA2_Stream0,ENABLE);//����DMA����  
}

void Drv_AdcInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef nvic;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADCʱ��
	
	ADC_DeInit();
	/*��ʼ��ADC1ͨ��1 ��IO��*/
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;/*ģ������*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 |GPIO_Pin_1 ;/*ͨ��0*/
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;/*����������*/
	GPIO_Init(GPIOA,&GPIO_InitStructure);/*��ʼ��*/
	/*ͨ�ÿ��ƼĴ���������*/
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMAʧ��*/
	ADC_CommonInitStructure.ADC_Mode          = ADC_Mode_Independent;/*����ģʽ*/
	ADC_CommonInitStructure.ADC_Prescaler     = ADC_Prescaler_Div4;/*APB2��4��Ƶ ��84/4=21M*/
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;/*���������׶ε���ʱ5��ʱ��*/
	ADC_CommonInit(&ADC_CommonInitStructure);/*��ʼ��*/
	/*��ʼ��ADC1*/
	ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;/*12λģʽ*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;/*ɨ��ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;/*����ת��*/
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*��ֹ������� ʹ���������*/
	ADC_InitStructure.ADC_DataAlign    = ADC_DataAlign_Right;/*�Ҷ���*/
	ADC_InitStructure.ADC_ExternalTrigConv = 0;
	ADC_InitStructure.ADC_NbrOfConversion = 2;/*2ͨ�� 1*/
	
	ADC_Init(ADC1,&ADC_InitStructure);/*��ʼ��*/
	
	initAdcDMA();

	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_15Cycles);/*���ù���ͨ��3 �������� ����ʱ�� */
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_15Cycles);/*���ù���ͨ��3 �������� ����ʱ�� */
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//��ADC�ж�
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);//Դ���ݱ仯ʱ����DMA����
	ADC_Cmd(ADC1,ENABLE);/*����ת��*/
	
	
	
	ADC_DMACmd(ADC1,ENABLE);//ʹ��ADC����
	
	
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

