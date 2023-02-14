#ifndef _PARAMETER_H
#define	_PARAMETER_H

#include "stm32f4xx.h"


__packed struct Parameter_s
{
	u16 frist_init;	//飞控第一次初始化，需要做一些特殊工作，比如清空flash
	
	
	u8	pwmInMode;				//接收机模式，分别为PWM型PPM型
	u8	heatSwitch;				//
	char testS[10];
	
	

};

union Parameter
{
	//这里使用联合体，长度是4KByte，联合体内部是一个结构体，该结构体内是需要保存的参数
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

