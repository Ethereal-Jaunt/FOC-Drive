#include "Parameter.h"
#include "Stmflash.h"
#include "usart.h"

//要写入的字符串数组
const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)	 		  	//数组长度
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)

#define FLASH_SAVE_ADDR  0X08060020 	//FLASH保存地址（必须为偶数，且所在扇区要大于本代码所占用的扇区，否则写操作的时候可能会擦除部分代码）
// STM32F407VETE FLASH有512K
// 扇区0、1、2、3均是16KB
// 扇区4 ： 0X0801 0000 -0x0801 FFFF 64KB
// 扇区5 ： 0X0802 0000 -0x0803 FFFF 128KB
// 扇区6 ： 0X0804 0000 -0x0805 FFFF 128KB
// 扇区7 ： 0X0806 0000 -0x0807 FFFF 128KB

union Parameter Parame;

//复位PID参数为默认值
void PID_Reset()
{
	

}


void Parame_Reset(void)
{
	//参数初始化
	

}


// 参数写入flash，即保存参数
// 写参数很慢，注意写的时机
void Parame_Write(void)
{
	Parame.set.frist_init = 12;
	Parame.set.pwmInMode = 1;
	Parame.set.testS[0] = 'h';
	Parame.set.testS[1] = 'e';

	STMFLASH_Write(FLASH_SAVE_ADDR,(u32*)Parame.byte,128);

	printf("write finish\r\n");
}

//从flash中读取参数
void Parame_Read(void)
{
	
	STMFLASH_Read(FLASH_SAVE_ADDR,(u32*)Parame.byte,128);
	
	printf("first init : %d pwnInmode : %d String: %s\r\n",Parame.set.frist_init,Parame.set.pwmInMode,Parame.set.testS);
}

/*延时存储任务*/
void Parame_Save_task(u16 dT_ms)
{
	//因为写入flash耗时较长，我们飞控做了一个特殊逻辑，在解锁后，是不进行参数写入的，此时会置一个需要写入标志位，等飞机降落锁定后，再写入参数，提升飞行安全性
	//为了避免连续更新两个参数，造成flash写入两次，我们飞控加入一个延时逻辑，参数改变后一秒，才进行写入操作，可以一次写入多项参数，降低flash擦写次数
	if(para_sta.save_en )				//允许存储
	{
		
	}
	else
	{

	}
}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
