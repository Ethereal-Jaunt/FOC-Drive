#ifndef __IOS_H
#define __IOS_H

#include "stm32f4xx.h"

/*---------------------- LED���ú� ------------------------*/

#define M0_EN1_PIN             GPIO_Pin_0       		 // ����    
#define M0_EN2_PIN             GPIO_Pin_1       		 //  ����  
#define M0_EN3_PIN             GPIO_Pin_2       		 //  ����  
#define M0_EN1_PORT            GPIOE                  // GPIO�˿�     
#define M0_EN2_PORT            GPIOE                  // GPIO�˿�   
#define M0_EN3_PORT            GPIOE                  //GPIO�˿�   
#define M0_EN1_CLK             RCC_AHB1Periph_GPIOE	 // GPIO�˿�ʱ��
#define M0_EN2_CLK             RCC_AHB1Periph_GPIOE	 // GPIO�˿�ʱ��
#define M0_EN3_CLK             RCC_AHB1Periph_GPIOE	 // GPIO�˿�ʱ��

/*---------------------- LED���ƺ� ------------------------*/
					
#define M0_EN1_OFF 	  GPIO_ResetBits(M0_EN1_PORT,M0_EN1_PIN);	// ����͵�ƽ	
#define M0_EN1_ON 	  GPIO_SetBits(M0_EN1_PORT,M0_EN1_PIN);		// ����ߵ�ƽ
#define M0_EN2_OFF 	  GPIO_ResetBits(M0_EN2_PORT,M0_EN2_PIN);	// ����͵�ƽ	
#define M0_EN2_ON 	  GPIO_SetBits(M0_EN2_PORT,M0_EN2_PIN);		// ����ߵ�ƽ
#define M0_EN3_OFF 	  GPIO_ResetBits(M0_EN3_PORT,M0_EN3_PIN);	// ����͵�ƽ
#define M0_EN3_ON 	  GPIO_SetBits(M0_EN3_PORT,M0_EN3_PIN);		// ����ߵ�ƽ

/*---------------------- �������� ----------------------------*/

void IO_Init(void);	//LED��ʼ������


#endif //__LED_H

