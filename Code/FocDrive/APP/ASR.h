#ifndef __ASR_H
#define __ASR_H

#include "stm32f4xx.h"
#include "arm_math.h"  


void ASR_PID_Init(void);
void ASR_calc(float32_t err_speed);



#endif

