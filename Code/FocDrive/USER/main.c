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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		//�ж����ȼ��������
	SysTick_Configuration(); 		//�δ�ʱ��
	Delay_ms(100);
	//delay_init(168);
	LED_Init();
	IO_Init();
	uart_init(115200);
	TIM1_PWM_Init(4200,1);			//2��Ƶ��10kHz������0.1ms
	I2C_Config();
	Drv_AdcInit();
	
	printf("Hardware Init Finish\r\n");
	Scheduler_Setup();				//��������ʼ����ϵͳΪ�㱼�������˹�����һ��ʱ�ֵ�����
	 //Set_Pwm(1050,2100,3150);
	while(1)
	{
		Scheduler_Run();			//�������������������ϵͳ���ܣ������жϷ�������������������������
	}

	
	
}