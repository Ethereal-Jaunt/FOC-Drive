#ifndef __FOC_H
#define __FOC_H

#include "stm32f4xx.h"
#include "arm_math.h"  



void Run_OpenLoop(void);

struct feedback{
	//µçÁ÷·´À¡
	float current_u;				
	float current_v;				
	float current_w;				
};

typedef	struct feedback	feedback_t;

#endif

