#include "ACR.h"
#include "foc.h"

arm_pid_instance_f32 ACR_PID_d,ACR_PID_q;

extern controlIO_t ctIO;

//pid parameter init
void ACR_PID_Init(void)
{
	ACR_PID_d.Kp = 18;
	ACR_PID_d.Ki = 3;
	ACR_PID_d.Kd = 0;
	
	ACR_PID_q.Kp = 18;
	ACR_PID_q.Ki = 3;
	ACR_PID_q.Kd = 0;

	
	arm_pid_init_f32(&ACR_PID_d,1);
	arm_pid_init_f32(&ACR_PID_q,1);
	
	arm_pid_reset_f32(&ACR_PID_d);
	arm_pid_reset_f32(&ACR_PID_q);
}


//err_d = expected value - feedback value
// err_q same as err_d
void ACR_calc(float32_t err_d ,float32_t err_q)
{
	
	ctIO.out_Ud = arm_pid_f32(&ACR_PID_d, err_d);
	ctIO.out_Uq = arm_pid_f32(&ACR_PID_q, err_q);
	
	if (ctIO.out_Ud >= 10)
		ctIO.out_Ud = 10;
	if(ctIO.out_Uq >= 10)
		ctIO.out_Uq = 10;
}


