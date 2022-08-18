#ifndef __SVPWM_H
#define __SVPWM_H

/* =================================================================================
File name:       __SVPWM_H.H  
===================================================================================*/

#include "stm32f4xx.h"
//#include "IQmathLib.h"
#include "arm_math.h"  

#define SV_DUTY		10
#define SV_MAX		95
#define SV_MIN		1

struct sv_mod{ 
	float32_t  Ualpha; 			// Input: reference alpha-axis phase voltage 
	float32_t  Ubeta;				// Input: reference beta-axis phase voltage 
	float32_t  Ta;				// Output: reference phase-a switching function		
	float32_t  Tb;				// Output: reference phase-b switching function 
	float32_t  Tc;				// Output: reference phase-c switching function
	float32_t  tmp1;				// Variable: temp variable
	float32_t  tmp2;				// Variable: temp variable
	float32_t  tmp3;				// Variable: temp variable
	uint16_t VecSector;		// Space vector sector
};
typedef struct 	sv_mod sv_mod_t;

struct sv_regs_mod{
	int32_t ccr1;				// Output: pwm compare register 1
	int32_t ccr2;				// Output: pwm compare register 2
	int32_t ccr3;				// Output: pwm compare register 3
};

typedef	struct sv_regs_mod	sv_regs_mod_t;

/*-----------------------------------------------------------------------------
Default initalizer for the SVGEN object.
-----------------------------------------------------------------------------*/                     
#define SVGEN_DEFAULTS { 0,0,0,0,0 }                       
extern sv_mod_t sv;
/*------------------------------------------------------------------------------
	Space Vector  Generator (SVGEN) Macro Definition
------------------------------------------------------------------------------*/

#define SVGENDQ_MACRO(v)														\
	v.tmp1= v.Ubeta;															\
	v.tmp2= (v.Ubeta)/2 + 0.866*v.Ualpha;					\
    v.tmp3= v.tmp2 - v.tmp1;													\
																				\
	v.VecSector=3;																\
	v.VecSector=(v.tmp2> 0)?( v.VecSector-1):v.VecSector;						\
	v.VecSector=(v.tmp3> 0)?( v.VecSector-1):v.VecSector;						\
	v.VecSector=(v.tmp1< 0)?(7-v.VecSector) :v.VecSector;						\
																				\
	if     (v.VecSector==1 || v.VecSector==4)                                   \
      {     v.Ta= v.tmp2; 														\
      		v.Tb= v.tmp1-v.tmp3; 												\
      		v.Tc=-v.tmp2;														\
      }								    										\
   																				\
    else if(v.VecSector==2 || v.VecSector==5)                                   \
      {     v.Ta= v.tmp3+v.tmp2; 												\
      		v.Tb= v.tmp1; 														\
      		v.Tc=-v.tmp1;														\
      }																	   		\
   																				\
    else                                                                        \
      {     v.Ta= v.tmp3; 														\
      		v.Tb=-v.tmp3; 														\
      		v.Tc=-(v.tmp1+v.tmp2);												\
      }																	   		\
																				\

//end of SVGENDQ_MACRO
	 
void svpwm_calc(sv_mod_t* p);
sv_regs_mod_t svpwm_get_regs_mod(int32_t vdc,int32_t pwm_t, sv_mod_t* v);
void svpwm_update(int32_t vdc, sv_mod_t* p);
	  
#endif


