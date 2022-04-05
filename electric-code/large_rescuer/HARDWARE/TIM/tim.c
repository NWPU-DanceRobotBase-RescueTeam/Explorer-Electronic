/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
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
  * @brief  定时器4初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM4_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  ///使能TIM4时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//初始化TIM4

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4, ENABLE); //使能定时器4

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  定时器4中断服务函数
  * @param  void
  * @retval void
  */

void TIM4_IRQHandler(void)   //TIM5中断
{
	u8 i;
	static u8 j = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM5更新中断标志 

		Maxon_Speed_Control();

		for (i = 0; i < 4; i++)
		{
			Motor_Speed_Control(M3508[i].PID.Goal_Speed, i);
		}
		CAN2_Send_Control_Value();
		
		if (j++ > 50)
		{
			j=0;
//			printf("左:%d",MAXON_MOTOR[0]);
//			printf("右:%d",MAXON_MOTOR[1]);
//            printf("\r\n");
//			Report_Current();
//			Report_ADC();
		}
	}
}

/**
  * @brief  定时器4中断服务函数
  * @param  void
  * @retval void
  */

//u8 times_numb=0,times_numb1=0;

//void TIM4_IRQHandler(void)   //TIM4中断
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIM4更新中断标志 
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
  * @brief  定时器5初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM5_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///使能TIM5时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//初始化TIM5

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //允许定时器5更新中断
	TIM_Cmd(TIM5, ENABLE); //使能定时器5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  定时器5中断服务函数
  * @param  void
  * @retval void
  */

void TIM5_IRQHandler(void)   //TIM5中断
{
	u8 i;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIM5更新中断标志 

		Maxon_Speed_Control();

		for (i = 0; i < 4; i++)
		{
			Motor_Speed_Control(M3508[i].PID.Goal_Speed, i);
		}
		CAN2_Send_Control_Value();
	}
}

