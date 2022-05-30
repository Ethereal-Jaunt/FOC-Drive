

#include "IOs.h"  

// 函数： IO口初始化
//
void IO_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_AHB1PeriphClockCmd ( M0_EN1_CLK, ENABLE); 	//初始化GPIOG时钟	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度选择
	
	//初始化  引脚
	GPIO_InitStructure.GPIO_Pin = M0_EN1_PIN|M0_EN2_PIN|M0_EN2_PIN;	 
	GPIO_Init(M0_EN1_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(M0_EN1_PORT, M0_EN1_PIN|M0_EN2_PIN|M0_EN2_PIN);  //PE0,1,2输出低电平
}

