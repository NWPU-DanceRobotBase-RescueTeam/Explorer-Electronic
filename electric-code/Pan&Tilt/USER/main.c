#include <math.h>
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "IMU.h"
#include "AX12.h"
#include "queue.h"
#include "Angle.h"
#include "can.h"
#include "tim.h"
#include "dma.h"


//                               ID  Length Write Reg   position   speed      check
volatile u8 Roll[11] ={0xFF,0xFF,0x01,0x07, 0x03, 0x1E, 0x00,0x00, 0xff,0x03, 0x00 };//���0x01
volatile u8 Pitch[11]={0xFF,0xFF,0x02,0x07, 0x03, 0x1E, 0x00,0x00, 0xff,0x03, 0x00 };//���0x02
extern int Receive_Flag;	
extern float roll,pitch;
extern int actuator_cmd;
 
extern const u8 num_1[11];
extern const u8 num_2[11];



void nimingsizhou(float dat,u8 id) //����������λ�������ô���
{
	union ftc
	{
		float f;
		u8 t[4];
	} mid;
	u8 data[8];
	u16 sum = 0;
	u8 i;
	
	//����������λ�������ô���
	data[0] = 0x88;
	data[1] = id;
	data[2] = 4;
	mid.f = dat;
	data[3] =mid.t[3];
	data[4] =mid.t[2];
	data[5] =mid.t[1];
	data[6] =mid.t[0];
	for(i=0;i<7;i++)	
		sum = sum + data[i];		//����У��λ
	data[7]=(uint8_t)(sum);					
	for(i=0;i<8;i++)			//���������ᷢ�͸��º�ĽǶ�
	{	
		USART_SendData(USART1, data[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
	}
}

int main(void)
{ 
	u8 t;
	binary2float angle;			
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);
	delay_init(168);  //��ʼ����ʱ����
	uart1_init(115200);//��ʼ������1������Ϊ115200
	LED_Init();
	Angle_Init();
	TIM3_Int_Init(500-1,8400-1);		
	delay_ms(10);
	AX12_Init();
	delay_ms(1000);
	IMU_init();
	DMA_Config_A();
	Receive_Flag=0;	

	while(1) 
	{
		if(Receive_Flag==1)		
		{
			Receive_Flag=0;	
			for(t=0;t<12;t++)
			{
				angle.binary[t] = ReceiveBuff[t+4];
			}
			for(t=0;t<3;t++)
				Angle[t] = angle.FloatData[t];
			AngleIsFinite();
			
			if(actuator_cmd==1)
			{
				AngleControl(Angle[ROLL]+2.0f,&RollAngle);  //������
				AngleControl(Angle[PITCH],&PitchAngle);   //������
				AngleUpdate();
				for(t=0;t<11;t++)
				{	
					USART_SendData(USART3, Roll[t]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
				}
				for(t=0;t<11;t++)			
				{	
					USART_SendData(USART3, Pitch[t]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
				}
			}
			else
			{
				for(t=0;t<11;t++)
				{	
					USART_SendData(USART3, num_1[t]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
				}
				for(t=0;t<11;t++)
				{
					USART_SendData(USART3, num_2[t]);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���	
				}
			}
		}
	}
}


