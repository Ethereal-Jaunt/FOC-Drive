#include "stm32f4xx.h"
#include "DataScope_DP.h"
#include "Foc.h"


extern expected_t ex;
extern feedback_t fb;

unsigned char DataScope_OutPut_Buffer[42] = {0};	   //���ڷ��ͻ�����


//����˵�����������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ 
//����˵�����û�����ֱ�Ӳ����˺��� 
//target:Ŀ�굥��������
//buf:��д������
//beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
//�����޷��� 
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //�õ�float�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
 
 
//����˵������������ͨ���ĵ����ȸ�������д�뷢�ͻ�����
//Data��ͨ������
//Channel��ѡ��ͨ����1-10��
//�����޷��� 
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
	if ( (Channel > 10) || (Channel == 0) ) return;  //ͨ����������10�����0��ֱ����������ִ�к���
  else
  {
     switch (Channel)
		{
      case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,0); break;
      case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,4); break;
		  case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,8); break;
		  case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,12); break;
		  case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,16); break;
		  case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,20); break;
		  case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,24); break;
		  case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,28); break;
		  case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,32); break;
		  case 10: Float2Byte(&Data,DataScope_OutPut_Buffer,36); break;
		}
  }	 
}


//����˵�������� DataScopeV1.0 ����ȷʶ���֡��ʽ
//Channel_Number����Ҫ���͵�ͨ������
//���ط��ͻ��������ݸ���
//����0��ʾ֡��ʽ����ʧ�� 
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
	if ( (Channel_Number > 10) || (Channel_Number == 0) ) { return 0; }  //ͨ����������10�����0��ֱ����������ִ�к���
  else
  {
		DataScope_OutPut_Buffer[Channel_Number*4 + 0] = 0x0; 
		DataScope_OutPut_Buffer[Channel_Number*4 + 1] = 0x0; 
		DataScope_OutPut_Buffer[Channel_Number*4 + 2] = 0x80; 
		DataScope_OutPut_Buffer[Channel_Number*4 + 3] = 0x7F; 
		
	 switch(Channel_Number)   
   { 
		 case 1:  return  8;  
		 case 2:  return 12;
		 case 3:  return 16; 
		 case 4:  return 20;
		 case 5:  return 24;  
		 case 6:  return 28;
		 case 7:  return 32; 
		 case 8:  return 36; 
		 case 9:  return 40;
     case 10: return 44; 
   }	 
  }
	return 0;
}


void DataUpload(void)
{
			u16 Send_Count,i;
	
			DataScope_Get_Channel_Data(fb.theta, 1 );
			DataScope_Get_Channel_Data(fb.elec_theta, 2 );
			

			Send_Count = DataScope_Data_Generate(2);
	
			for( i = 0 ; i < Send_Count; i++) 
			{
			while((USART1->SR&0X40)==0);  
			USART1->DR = DataScope_OutPut_Buffer[i]; 
			}
}







