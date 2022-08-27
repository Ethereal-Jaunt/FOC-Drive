#include "ASR.h"
#include "foc.h"

arm_pid_instance_f32 ASR_PID;

extern controlIO_t ctIO;

//pid parameter init
void ASR_PID_Init(void)
{
	ASR_PID.Kp = 10;
	ASR_PID.Ki = 0.05;
	ASR_PID.Kd = 0;

	arm_pid_init_f32(&ASR_PID,1);
	
	arm_pid_reset_f32(&ASR_PID);
}



void ASR_calc(float32_t err_speed)
{
	ctIO.out_iq = arm_pid_f32(&ASR_PID, err_speed);
	
	if(ctIO.out_iq >= 2)
		ctIO.out_Uq = 2;
}





