#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_
#include "stm32f4xx.h"

extern __IO uint16_t AdcValue[2];

void Drv_AdcInit(void);

#endif
