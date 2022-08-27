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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		//�ж����ȼ��������
	SysTick_Configuration(); 		//�δ�ʱ��
	Delay_ms(100);
	//delay_init(168);
	LED_Init();
	IO_Init();
	KEY_Init();
	uart_init(256000);
	TIM1_PWM_Init(4200,1);			//2��Ƶ��10kHz������0.1ms
	I2C_Config();
	Drv_AdcInit();
	ACR_PID_Init();	//pid parameter init
	ASR_PID_Init();
	
	printf("Hardware Init Finish\r\n");
	Scheduler_Setup();				//��������ʼ����ϵͳΪ�㱼�������˹�����һ��ʱ�ֵ�����
	//Set_Pwm(2100,4200,6300);
	while(1)
	{
		Scheduler_Run();			//�������������������ϵͳ���ܣ������жϷ�������������������������
	}

	
	
}


