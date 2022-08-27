#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "IOs.h"
#include "timer.h"
#include "Scheduler.h"
#include "Drv_time.h"
#include "Drv_i2c.h"
#include "Drv_adc.h"
#include "ACR.h"
#include "ASR.h"
#include "key.h"


u8 iostatePE9,iostatePE11,iostatePE13,iostatePE14;
u8 iostatePC8;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		//中断优先级组别设置
	SysTick_Configuration(); 		//滴答时钟
	Delay_ms(100);
	//delay_init(168);
	LED_Init();
	IO_Init();
	KEY_Init();
	uart_init(256000);
	TIM1_PWM_Init(4200,1);			//2分频，10kHz，周期0.1ms
	I2C_Config();
	Drv_AdcInit();
	ACR_PID_Init();	//pid parameter init
	ASR_PID_Init();
	
	printf("Hardware Init Finish\r\n");
	Scheduler_Setup();				//调度器初始化，系统为裸奔，这里人工做了一个时分调度器
	//Set_Pwm(2100,4200,6300);
	while(1)
	{
		Scheduler_Run();			//运行任务调度器，所有系统功能，除了中断服务函数，都在任务调度器内完成
	}

	
	
}


