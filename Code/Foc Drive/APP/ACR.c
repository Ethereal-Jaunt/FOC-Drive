#include "ACR.h"


arm_pid_instance_f32 ACR_PID;

//pid parameter init
void ACR_PID_Init()
{
	ACR_PID.Kp = 1;
	ACR_PID.Ki = 1;
	ACR_PID.Kp = 0;

	arm_pid_init_f32(&ACR_PID,1);
	
	arm_pid_reset_f32(&ACR_PID);
}



void ACR_calc(float32_t sample)
{
	
	arm_pid_f32(&ACR_PID, sample);

}


