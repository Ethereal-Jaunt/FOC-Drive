#ifndef __FOC_H
#define __FOC_H

#include "stm32f4xx.h"
#include "arm_math.h"  



struct feedback{
	//��������
	float current_u;				
	float current_v;				
	float current_w;	
	float current_d;				
	float current_q;
	//λ�÷���
	float theta;//��е�Ƕ�
	float last_theta;//��е�Ƕ�
	float elec_theta;//��Ƕ�
	float elec_theta_rad;
	float speed;//RPM
};

typedef	struct feedback	feedback_t;

struct expected{

	float current_d;				
	float current_q;
	
	float position;//��е�Ƕ�
	float speed;	//RPM
	 
};

typedef	struct expected	expected_t;


struct controlIO{	
	
	float out_Ud;	//ACR output				
	float out_Uq;	//ACR output
	
	float out_iq;	//ASR output
	
	float out_speed;	//APR output
	
	
	 
};

typedef	struct controlIO	controlIO_t;

void IuvwToIdq(float32_t Iu,float32_t Iv,float32_t *Id,float32_t *Iq ,float32_t theta);
void Run_OpenLoop(void);
void Run_CloseLoop(void);
void Run_RotateVector(void);

#endif

