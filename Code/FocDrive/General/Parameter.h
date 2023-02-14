#ifndef _PARAMETER_H
#define	_PARAMETER_H

#include "stm32f4xx.h"


__packed struct Parameter_s
{
	u16 frist_init;	//�ɿص�һ�γ�ʼ������Ҫ��һЩ���⹤�����������flash
	
	
	u8	pwmInMode;				//���ջ�ģʽ���ֱ�ΪPWM��PPM��
	u8	heatSwitch;				//
	char testS[10];
	
	

};

union Parameter
{
	//����ʹ�������壬������4KByte���������ڲ���һ���ṹ�壬�ýṹ��������Ҫ����Ĳ���
	struct Parameter_s set;
	u8 byte[4096];
};
extern union Parameter Parame;

typedef struct
{
	u8 save_en;
	u8 save_trig;
	u16 time_delay;
}_parameter_state_st ;
extern _parameter_state_st para_sta;

void Parame_Read(void);
void Parame_Write(void);
void Parame_Save_task(u16 dT_ms);
void PID_Reset(void);
void Parame_Reset(void);

#endif 

