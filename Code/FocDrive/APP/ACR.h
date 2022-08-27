#ifndef __ACR_H
#define __ACR_H

#include "stm32f4xx.h"
#include "arm_math.h" 

void ACR_PID_Init(void);
void ACR_calc(float32_t err_d ,float32_t err_q );


#endif
