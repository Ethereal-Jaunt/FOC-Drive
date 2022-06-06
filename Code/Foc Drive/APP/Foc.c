#include "Foc.h"
#include "svpwm.h"
#include "timer.h"



sv_mod_t foc_var;
sv_regs_mod_t foc_reg_var;

//由Ud,Uq计算得到 U_alpha , U_beta
void UdqToUab(float32_t Ud,float32_t Uq,float32_t *U_alpha,float32_t *U_beta,float32_t theta)
{
	float32_t sinTheta,cosTheta;
	sinTheta = arm_sin_f32(theta);
	cosTheta = arm_cos_f32(theta);
	
	arm_inv_park_f32(Ud,Uq,U_alpha,U_beta,sinTheta,cosTheta);

}

void Run_OpenLoop()
{
	static float32_t theta = 0;
	
	UdqToUab(0,1, &foc_var.Ualpha , &foc_var.Ubeta , theta );
	
	svpwm_calc(&foc_var);
	
	foc_reg_var = svpwm_get_regs_mod(12,10,&foc_var);
	
	Set_Pwm(foc_reg_var.ccr1,foc_reg_var.ccr2,foc_reg_var.ccr3);
	
	theta += 0.1;
	if ( theta > 360 )
		theta = theta - 360;
}

//3s-dq变换 将采样的电流值转换为Id,Iq
void IuvwToIdq(float32_t Iu,float32_t Iv,float32_t *Id,float32_t *Iq ,float32_t theta)
{
	float32_t I_alpha , I_beta;
	float32_t sinTheta,cosTheta;
	
	sinTheta = arm_sin_f32(theta);
	cosTheta = arm_cos_f32(theta);
	
	//3s -> 2s
	arm_clarke_f32( Iu ,Iv , &I_alpha , &I_beta);
	//2s -> dq
	arm_park_f32(I_alpha,I_beta,Id,Iq,sinTheta,cosTheta);
	
}
