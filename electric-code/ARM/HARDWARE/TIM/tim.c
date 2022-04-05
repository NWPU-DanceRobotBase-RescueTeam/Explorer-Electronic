/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
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
	static u8 i = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
		
		if (Motor[0].Start_Flag == 1)
		{
			Angle_Position_Control(0);
			Motor_Speed_Control(Motor[0].PID.Goal_Speed, 0);
		}
		else
			Motor_Speed_Control(0, 0);	//已到指定位置或不需移动，速度为0
		
		
//		get_pot_adc_angle();
//		Angle_Position_Control(0);
//		Motor_Speed_Control(Motor[0].PID.Goal_Speed, 0);
		
//		Motor_Position_Control(SECOND_ARM_START - second_arm_goal_postion, 0);
		
		CAN_Send_Control_Value();
		
		if (i++ > 10)
		{
			printf("角度= %f\r\n", Motor[0].Current_angle);
			printf("目标= %f\r\n", SECOND_ARM_START - second_arm_goal_postion);
			printf("速度= %d\r\n", Motor[0].Control_Current);
			printf("Es= %f %d\r\n", Motor[0].PID.Error_Sum, Motor[0].Start_Flag);
			i = 0;
		}
	}
}

