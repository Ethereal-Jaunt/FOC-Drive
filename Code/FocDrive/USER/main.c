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

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		//中断优先级组别设置
	SysTick_Configuration(); 		//滴答时钟
	Delay_ms(100);
	//delay_init(168);
	LED_Init();
	IO_Init();
	uart_init(115200);
	TIM1_PWM_Init(4200,1);			//2分频，10kHz，周期0.1ms
	I2C_Config();
	Drv_AdcInit();
	
	printf("Hardware Init Finish\r\n");
	Scheduler_Setup();				//调度器初始化，系统为裸奔，这里人工做了一个时分调度器
	 //Set_Pwm(1050,2100,3150);
	while(1)
	{
		Scheduler_Run();			//运行任务调度器，所有系统功能，除了中断服务函数，都在任务调度器内完成
	}

	
	
}