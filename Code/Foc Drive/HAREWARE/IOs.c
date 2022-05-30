

#include "IOs.h"  

// ������ IO�ڳ�ʼ��
//
void IO_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_AHB1PeriphClockCmd ( M0_EN1_CLK, ENABLE); 	//��ʼ��GPIOGʱ��	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶ�ѡ��
	
	//��ʼ��  ����
	GPIO_InitStructure.GPIO_Pin = M0_EN1_PIN|M0_EN2_PIN|M0_EN2_PIN;	 
	GPIO_Init(M0_EN1_PORT, &GPIO_InitStructure);	
	
	GPIO_ResetBits(M0_EN1_PORT, M0_EN1_PIN|M0_EN2_PIN|M0_EN2_PIN);  //PE0,1,2����͵�ƽ
}

