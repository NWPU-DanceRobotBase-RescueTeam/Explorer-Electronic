#include "pwm.h"

/**
  * @brief  定时器TIM4,CH1,CH2 设置为PWM模式
  * @param  arr：自动重装值
  *         psc：时钟预分频数
  * @retval void
  */

void TIM4_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
    //定时器4的CH1,CH2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  	//TIM4时钟使能    					
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4); //GPIOB6复用为定时器4
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4); //GPIOB7复用为定时器4	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;           //GPIOB6,B7					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化PB6,PB7
	  
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//初始化定时器4
	
	//初始化TIM4 CH1,CH2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:PWM模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC1
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4OC2

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
										  
}  

/**
  * @brief  定时器TIM3,CH3,CH4 设置为PWM模式
  * @param  arr：自动重装值
  *         psc：时钟预分频数
  * @retval void
  */

void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	//定时器3的 CH3,CH4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //GPIOB0复用为定时器3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); //GPIOB1复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;    //GPIOB0,B1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);              //初始化PB0,PB1
	  
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//初始化定时器3
	
	//初始化TIM3 CH3,CH4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:PWM模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC3
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC4

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	
} 

/**
  * @brief  定时器TIM2,CH2 设置为PWM模式
  * @param  arr：自动重装值
  *         psc：时钟预分频数
  * @retval void
  */

void TIM2_PWM_Init(u32 arr,u32 psc)
{		 					 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//定时器2的通道2 CH2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  	//TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2); //GPIOB3复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOB3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PB3
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//初始化定时器2
	
	//初始化TIM3 CH2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:PWM模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3OC2

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器

	TIM_ARRPreloadConfig(TIM2, ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM3
 
										  
}

