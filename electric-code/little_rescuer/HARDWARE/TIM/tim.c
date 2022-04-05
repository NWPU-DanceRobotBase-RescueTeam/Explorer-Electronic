/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#include "tim.h"
#include "sensor.h"
#include "debug.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "led.h"
#include "iwdg.h"

/**
  * @brief  定时器3初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

/**
  * @brief  定时器3中断服务函数
  * @param  void
  * @retval void
  */

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
			
		Motor_Speed_Control(M3508[0].PID.Goal_Speed, 0);
		Motor_Speed_Control(M3508[1].PID.Goal_Speed, 1);
		Motor_Speed_Control(M3508[2].PID.Goal_Speed, 2);
		Motor_Speed_Control(M3508[3].PID.Goal_Speed, 3);
		CAN_Send_Control_Value();
		
	}
}

/**
  * @brief  定时器5初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM5_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = arr;		 /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler = psc;	/* 时钟预分频数 72M/20000 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM5, ENABLE);
}

/**
  * @brief  定时器3中断服务函数
  * @param  void
  * @retval void
  */

void TIM5_IRQHandler(void)   //TIM3中断
{
	static int i=0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
		
		LED0=!LED0;
		
		Sonic_Trig();
		Flame_Calculate();
		Rain_Calculate();
		Infrare_Calculate();
		Smoke_Calculate();
		Light_Calculate();
		
			
		if (i++ >= 20)
		{
			i = 0;
	//		if(Distance<50||flame<50||rain<70||infrare>50||smoke>50)
			if (flame < 50 || rain < 70)
				buzzer = 1;
			else
				buzzer = 0;
			if (light > 50)
				lamp = 1;
			else
				lamp = 0;
				
	//		EXTI_GenerateSWInterrupt(EXTI_Line0);
			Distance = 500;
			flame = 100;
			rain = 100;
			infrare = 0;
			smoke = 0;
			light = 0;
			
	//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
	//			printf("yes\r\n");
				
	//		printf("pitch=%f\r\n",pitch);
	//		printf("roll=%f\r\n",roll);
	//		printf("yaw=%f\r\n",yaw);
				
	//		printf("Distance=%f\r\n",Distance);
	//		printf("flame=%d\r\n",flame);
	//		printf("rain=%d\r\n",rain);
	//		printf("infrare=%d\r\n",infrare);
	//		printf("smoke=%d\r\n",smoke);
	//		printf("light=%d\r\n",light);
		}
	}
}

/**
  * @brief  定时器4初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM4, TIM_IT_Update,DISABLE);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	//更新产生中断
}

/**
  * @brief  定时器2初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM2_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOA时钟
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);

	//设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

    //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIM3
}

/**
  * @brief  定时器7初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM7_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM7, ENABLE);  //使能TIMx					 
}

/**
  * @brief  定时器7中断服务函数
  * @param  void
  * @retval void
  */

void TIM7_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx更新中断标志
		if (change == 0)
			angle += 1;
		else
			angle -= 1;
		if (angle >= 1900)
			change = 1;
		else if (angle <= 1750)
			change = 0;
		TIM_SetCompare2(TIM2, angle);
//		delay_ms(1);
	}
}

