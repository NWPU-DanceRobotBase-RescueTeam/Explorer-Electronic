/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "tim.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "pot.h"
#include "arm.h"
#include "Motor_Control.h"
#include "can.h"
#include "debug.h"

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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //�����ȼ�3
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
	static u8 i = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		
		if (Motor[0].Start_Flag == 1)
		{
			Angle_Position_Control(0);
			Motor_Speed_Control(Motor[0].PID.Goal_Speed, 0);
		}
		else
			Motor_Speed_Control(0, 0);	//�ѵ�ָ��λ�û����ƶ����ٶ�Ϊ0
		
		
//		get_pot_adc_angle();
//		Angle_Position_Control(0);
//		Motor_Speed_Control(Motor[0].PID.Goal_Speed, 0);
		
//		Motor_Position_Control(SECOND_ARM_START - second_arm_goal_postion, 0);
		
		CAN_Send_Control_Value();
		
		if (i++ > 10)
		{
			printf("�Ƕ�= %f\r\n", Motor[0].Current_angle);
			printf("Ŀ��= %f\r\n", SECOND_ARM_START - second_arm_goal_postion);
			printf("�ٶ�= %d\r\n", Motor[0].Control_Current);
			printf("Es= %f %d\r\n", Motor[0].PID.Error_Sum, Motor[0].Start_Flag);
			i = 0;
		}
	}
}

