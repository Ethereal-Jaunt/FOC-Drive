#include "ASR.h"


arm_pid_instance_f32 ASR_PID;

//pid parameter init
void ASR_PID_Init()
{
	ASR_PID.Kp = 1;
	ASR_PID.Ki = 1;
	ASR_PID.Kp = 0;

	arm_pid_init_f32(&ASR_PID,1);
	
	arm_pid_reset_f32(&ASR_PID);
}



void ASR_calc(float32_t sample)
{
	
	arm_pid_f32(&ASR_PID, sample);

}





