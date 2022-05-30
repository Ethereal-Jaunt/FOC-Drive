#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "IOs.h"
#include "timer.h"
#include "Scheduler.h"
#include "Drv_time.h"
#include "Drv_i2c.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		//�ж����ȼ��������
	SysTick_Configuration(); 		//�δ�ʱ��
	Delay_ms(100);
	//delay_init(168);
	LED_Init();
	IO_Init();
	uart_init(115200);
	TIM1_PWM_Init(2500,168);			//20ms  pwm 50Hz
	I2C_Config();
	
	printf("Hardware Init Finish\r\n");
	Scheduler_Setup();				//��������ʼ����ϵͳΪ�㱼�������˹�����һ��ʱ�ֵ�����
	while(1)
	{
		Scheduler_Run();			//�������������������ϵͳ���ܣ������жϷ�������������������������
	}

	
	
}