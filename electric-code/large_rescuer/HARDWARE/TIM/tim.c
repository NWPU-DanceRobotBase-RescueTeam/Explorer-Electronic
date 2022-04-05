/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "tim.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "MAXON_Motor.h"
#include "led.h"
#include "adc.h"
#include "pot.h"
#include "debug.h"

/**
  * @brief  ��ʱ��4��ʼ��
  * @param  arr:��װ��ֵ
  *         psc: Ԥ��Ƶֵ
  * @retval void
  */

void TIM4_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  ///ʹ��TIM4ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//��ʼ��TIM4

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��4

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ��ʱ��4�жϷ�����
  * @param  void
  * @retval void
  */

void TIM4_IRQHandler(void)   //TIM5�ж�
{
	u8 i;
	static u8 j = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIM5�����жϱ�־ 

		Maxon_Speed_Control();

		for (i = 0; i < 4; i++)
		{
			Motor_Speed_Control(M3508[i].PID.Goal_Speed, i);
		}
		CAN2_Send_Control_Value();
		
		if (j++ > 50)
		{
			j=0;
//			printf("��:%d",MAXON_MOTOR[0]);
//			printf("��:%d",MAXON_MOTOR[1]);
//            printf("\r\n");
//			Report_Current();
//			Report_ADC();
		}
	}
}

/**
  * @brief  ��ʱ��4�жϷ�����
  * @param  void
  * @retval void
  */

//u8 times_numb=0,times_numb1=0;

//void TIM4_IRQHandler(void)   //TIM4�ж�
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIM4�����жϱ�־ 
//		if(times_numb++ >= 3)
//		{
//			MAXON_MOTOR[0] = 0;
//			MAXON_MOTOR[1] = 0;
//			times_numb =0;
//		}
//		if(times_numb1++ >= 2)
//		{
//			if(relay.GPIO_Mode == GPIO_Mode_AN) 
//				relay_ON();
//			times_numb1 =0;
//		}
//	}
//}

/**
  * @brief  ��ʱ��5��ʼ��
  * @param  arr:��װ��ֵ
  *         psc: Ԥ��Ƶֵ
  * @retval void
  */

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
	TIM_Cmd(TIM5, ENABLE); //ʹ�ܶ�ʱ��5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ��ʱ��5�жϷ�����
  * @param  void
  * @retval void
  */

void TIM5_IRQHandler(void)   //TIM5�ж�
{
	u8 i;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIM5�����жϱ�־ 

		Maxon_Speed_Control();

		for (i = 0; i < 4; i++)
		{
			Motor_Speed_Control(M3508[i].PID.Goal_Speed, i);
		}
		CAN2_Send_Control_Value();
	}
}

