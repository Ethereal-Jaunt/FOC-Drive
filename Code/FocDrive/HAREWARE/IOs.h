#ifndef __IOS_H
#define __IOS_H

#include "stm32f4xx.h"

/*---------------------- LED配置宏 ------------------------*/

#define M0_EN1_PIN             GPIO_Pin_0       		 // 引脚    
#define M0_EN2_PIN             GPIO_Pin_1       		 //  引脚  
#define M0_EN3_PIN             GPIO_Pin_2       		 //  引脚  
#define M0_EN1_PORT            GPIOE                  // GPIO端口     
#define M0_EN2_PORT            GPIOE                  // GPIO端口   
#define M0_EN3_PORT            GPIOE                  //GPIO端口   
#define M0_EN1_CLK             RCC_AHB1Periph_GPIOE	 // GPIO端口时钟
#define M0_EN2_CLK             RCC_AHB1Periph_GPIOE	 // GPIO端口时钟
#define M0_EN3_CLK             RCC_AHB1Periph_GPIOE	 // GPIO端口时钟

/*---------------------- LED控制宏 ------------------------*/
					
#define M0_EN1_OFF 	  GPIO_ResetBits(M0_EN1_PORT,M0_EN1_PIN);	// 输出低电平	
#define M0_EN1_ON 	  GPIO_SetBits(M0_EN1_PORT,M0_EN1_PIN);		// 输出高电平
#define M0_EN2_OFF 	  GPIO_ResetBits(M0_EN2_PORT,M0_EN2_PIN);	// 输出低电平	
#define M0_EN2_ON 	  GPIO_SetBits(M0_EN2_PORT,M0_EN2_PIN);		// 输出高电平
#define M0_EN3_OFF 	  GPIO_ResetBits(M0_EN3_PORT,M0_EN3_PIN);	// 输出低电平
#define M0_EN3_ON 	  GPIO_SetBits(M0_EN3_PORT,M0_EN3_PIN);		// 输出高电平

/*---------------------- 函数声明 ----------------------------*/

void IO_Init(void);	//LED初始化函数


#endif //__LED_H

