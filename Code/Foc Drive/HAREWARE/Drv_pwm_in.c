/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 描述    ：PWM输入捕获
**********************************************************************************/
#include "Drv_pwm_in.h"


#define PPM_CHNUM 8

u16 Rc_Ppm_In[PPM_CHNUM ];

void PWM_IN_Init ( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_StructInit(&GPIO_InitStructure);
	TIM_ICStructInit(&TIM_ICInitStructure);
	
    RCC_APB1PeriphClockCmd (  RCC_APB1Periph_TIM4, ENABLE );
    RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOD, ENABLE );


    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init ( &NVIC_InitStructure );

//////////////////////////////////////////////////////////////////////////////////////////////

	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
		GPIO_Init ( GPIOD, &GPIO_InitStructure );
		
		GPIO_PinAFConfig ( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4 );
		
		TIM4->PSC = ( 168 / 2 ) - 1;
		
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit ( TIM4, &TIM_ICInitStructure );
		
		TIM_Cmd ( TIM4, ENABLE );
		TIM_ITConfig ( TIM4, TIM_IT_CC1, ENABLE );


}

void _TIM4_IRQHandler ( void )
{
    static u16 temp_cnt1, temp_cnt1_2, temp_cnt2, temp_cnt2_2, temp_cnt3, temp_cnt3_2, temp_cnt4, temp_cnt4_2;

    if ( TIM4->SR & TIM_IT_CC1 )
    {
	
			
        TIM4->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        TIM4->SR = ~TIM_FLAG_CC1OF;
	
			static u8 ch_sta = 0;
			if ( GPIOD->IDR & GPIO_Pin_12 )
			{
				temp_cnt1 = TIM_GetCapture1 ( TIM4 );
			}
			else
			{
				temp_cnt1_2 = TIM_GetCapture1 ( TIM4 );
				u16 _tmp;
				if ( temp_cnt1_2 >= temp_cnt1 )
					_tmp = temp_cnt1_2 - temp_cnt1;
				else
					_tmp = 0xffff - temp_cnt1 + temp_cnt1_2 + 1;
				if(_tmp > 2100 || ch_sta == PPM_CHNUM )
				{
					ch_sta = 0;
				}
				else
				{
					Rc_Ppm_In[ch_sta] = _tmp;
					ch_sta++;
				}
			}
		
    }
	
}



