#include <string.h>
#include <math.h>
#include "tim.h"
#include "can.h"
#include "adc.h"

/**************************************************************************
�������ܣ���ʱ��3��ʼ��
��ڲ�����arr:��װ��ֵ , psc: Ԥ��Ƶֵ
����  ֵ����
**************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  			 
}


/**************************************************************************
�������ܣ���ʱ��3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
u16 adc = 0;

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
//		static u16 queue[20] ={0};
//		s8 i;
//		s8 error = 0;
//		u16 front = 0, rear = 0;
//		u8 sendmessage[8] = {0};
//		float x;
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
		
		adc = Get_Adc_Average(2, 5);
		
		printf("%d\r\n",adc);
		
//		x = (float)adc;
//		memcpy(sendmessage, &x, 4);
//		CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
		
//		for (i = 18; i >= 0; i--)
//		{
//			queue[i + 1] = queue[i];
//			if(i < 9)
//				front += queue[i];
//			else
//				rear += queue[i];
//		}
//		queue[0] = adc;
//		front += adc;
//		front /= 10;
//		rear /= 10;
//		error = front - rear;
//		if(error < -5)
//		{
//			LED0 = 1;
//			x = 1;
//		}
//		else
//		{
//			LED0 = 0;
//			x = 0;
//		}

//		if (adc < 2500)
//		{
//			LED0 = 1;
//			x = 1;
//		}
//		else
//		{
//			LED0 = 0;
//			x = 0;
//		}

		
//		printf("CO2:%d\r\n",adc);
	}
}

void Message_Control()
{
	CanRxMsg RxMessage;
	float cmd;
	u8 sendmessage[8] = {0};
	float x = 0;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	switch(RxMessage.StdId)
	{
		// ����������Ϣ
		case 0x09e0 >> 5 : 
		{
			memcpy(&cmd,RxMessage.Data+4,4);
			
			if(fabs(cmd -1.0f) < 0.1)
			{
				x = (float)adc;
				memcpy(sendmessage, &x, 4);
				CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
				printf("adc:  %d\r\n",adc);
				LED0 = ~LED0;
			}
			break;
		}
    
	}
}

void TIM5_Int_Init(u16 arr, u16 psc)		
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///ʹ��TIM5ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //����ʱ��5�����ж�
	TIM_Cmd(TIM5, DISABLE); //ʹ�ܶ�ʱ��5
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			 
}

void TIM5_IRQHandler(void)   //TIM5�ж�
{
	static u8 i = 0;
	float cmd;
	u8 sendmessage[8] = {0};
	float x = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		x = (float)adc;
		memcpy(sendmessage, &x, 4);
		CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
		printf("adc:  %d\r\n",adc);
		LED0 = ~LED0;
		if(i++ >10)
			NVIC_SystemReset();
	}
}
