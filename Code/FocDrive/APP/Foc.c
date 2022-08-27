#include "Foc.h"
#include "svpwm.h"
#include "timer.h"
#include "usart.h"
#include "as5600.h"
#include "ACR.h"
#include "ASR.h"


sv_mod_t foc_var;
sv_regs_mod_t foc_reg_var;
controlIO_t ctIO;

const float init_theta = 22.881001 ; //39.762817 ;	//0角度 31.580999 + 8.181818(360/11)  22.881001 54.984001
const float init_theta2  = 31.580999 ;


feedback_t fb;
expected_t ex;

//由Ud,Uq计算得到 U_alpha , U_beta
void UdqToUab(float32_t Ud,float32_t Uq,float32_t *U_alpha,float32_t *U_beta,float32_t theta)
{
	float32_t sinTheta,cosTheta;
	sinTheta = arm_sin_f32(theta);
	cosTheta = arm_cos_f32(theta);
	
	if (Ud >= 10)
		Ud = 10;
	if(Uq >= 10)
		Uq = 10;
	
	arm_inv_park_f32(Ud,Uq,U_alpha,U_beta,sinTheta,cosTheta);

}

void Run_OpenLoop(void)
{
	float temp1;
	
	fb.theta = Get_Angle(); //读取机械角度
	if(fb.theta >= init_theta)
		temp1 = fb.theta - init_theta ;
	else
		temp1 = 360 + fb.theta - init_theta ;
	fb.elec_theta = fmod(temp1*11,360.0);
	fb.elec_theta_rad = fb.elec_theta/180*PI;
	
	UdqToUab(0,1, &foc_var.Ualpha , &foc_var.Ubeta , fb.elec_theta_rad );
	//printf("%f\r\n",fb.theta);
	
	svpwm_calc(&foc_var);
	
	foc_reg_var = svpwm_get_regs_mod(12,8400,&foc_var);
	
	Set_Pwm(foc_reg_var.ccr1,foc_reg_var.ccr2,foc_reg_var.ccr3);
	
	IuvwToIdq(fb.current_u,fb.current_v,&fb.current_d,&fb.current_q,fb.elec_theta_rad);
	//printf("%.3f	%.3f\r\n",fb.current_d,fb.current_q);
}


void Run_CloseLoop(void)
{
	float temp1;
	
  fb.theta = Get_Angle(); //读取机械角度
	fb.speed = -(fb.theta - fb.last_theta)*500/3;	// RPM
	fb.last_theta = fb.theta ; 
	
	if(fb.theta >= init_theta)
		temp1 = fb.theta - init_theta ;
	else
		temp1 = 360 + fb.theta - init_theta ;
	fb.elec_theta = fmod( temp1*11,360.0) ;
	fb.elec_theta_rad = fb.elec_theta/180*PI;
	
	// Iu , Iv tranfer to Id , Iq
	IuvwToIdq(fb.current_u,fb.current_v,&fb.current_d,&fb.current_q,fb.elec_theta_rad);

	// Speed loop
	ASR_calc(ex.speed - fb.speed);
	// Current loop
	ex.current_q = ctIO.out_iq / 1000;
	ACR_calc(0 - fb.current_d , ex.current_q - fb.current_q);
	//printf("%.4f	%.4f\r\n",ex.current_q,ex.speed - fb.speed);
	// apply output of ACR to SVPWM
	printf("%.4f\r\n",ctIO.out_Uq);
	UdqToUab(ctIO.out_Ud ,ctIO.out_Uq, &foc_var.Ualpha , &foc_var.Ubeta , fb.elec_theta_rad );	// calculate Ua and Ub according Ud and Uq
	svpwm_calc(&foc_var);	//calculate Ta , Tb , Tc
	foc_reg_var = svpwm_get_regs_mod(12,8400,&foc_var);	//calculate ccr1 , ccr2 ,ccr3
	Set_Pwm(foc_reg_var.ccr1,foc_reg_var.ccr2,foc_reg_var.ccr3);	//set pwm  <-- final step
	
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




void Run_RotateVector(void)
{
	static float32_t theta = 0;
	float32_t theta_rad ;
	
	theta_rad = fmod(theta*11,360.0)/180*PI;
	
	UdqToUab(0,3, &foc_var.Ualpha , &foc_var.Ubeta , theta_rad );
	svpwm_calc(&foc_var);
	foc_reg_var = svpwm_get_regs_mod(12,8400,&foc_var);
	Set_Pwm(foc_reg_var.ccr1,foc_reg_var.ccr2,foc_reg_var.ccr3);
	
	theta += 0.18;
	if ( theta > 360 )
		theta = theta - 360;
}

