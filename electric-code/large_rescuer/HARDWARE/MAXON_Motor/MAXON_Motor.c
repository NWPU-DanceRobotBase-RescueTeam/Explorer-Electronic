#include <math.h>
#include "MAXON_Motor.h"

/**
  * @brief  MAXON电机初始化函数
  * @param  void
  * @retval void
  */

void Maxon_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  	//TIM2时钟使能   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM2时钟使能   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOC时钟

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2); //GPIOB10复用为定时器2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0复用为定时器2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  //初始化电机的数字输入端2、3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;  //初始化电机的数字输入端2、3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//电机使能
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	
	//电机正转
	LEFT_Direction = 1;
	RIGHT_Direction = 1;

	//数字输入端1初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOB10
	GPIO_Init(GPIOB, &GPIO_InitStructure);               //初始化PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//GPIOA0
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化PA0

	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz

	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 500 - 1;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//初始化定时器2
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//初始化定时器2

	//初始化PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器

	TIM_ARRPreloadConfig(TIM2, ENABLE);//ARPE使能 
	TIM_ARRPreloadConfig(TIM5, ENABLE);//ARPE使能

	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
	TIM_Cmd(TIM5, ENABLE);  //使能TIM2
}

/**
  * @brief  MAXON速度设置
  * @param  void
  * @retval void
  */

s16 MAXON_MOTOR[2]; //MAXON电机速度

void Maxon_Speed_Control(void)  // pwm值：10Hz--5kHz
{
	u16 pwm = 0;

	pwm = 50 + fabs(MAXON_MOTOR[0]) / 10;

	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[0] >= 0)
		LEFT_Direction = 1;
	else
		LEFT_Direction = 0;

	LEFT_Speed(pwm);

	pwm = 50 + fabs(MAXON_MOTOR[1]) / 10;
	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[1] >= 0)
		RIGHT_Direction = 1;
	else
		RIGHT_Direction = 0;

	RIGHT_Speed(pwm);
}

