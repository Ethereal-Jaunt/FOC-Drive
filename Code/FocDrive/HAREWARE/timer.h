#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx.h"

void TIM1_PWM_Init(u32 arr,u32 psc);
void Set_Pwm(int pwm1 , int pwm2 ,int pwm3);
void TIM3_Int_Init(u16 arr,u16 psc);


#endif



