#include "sensor.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"

/**
  * @brief  传感器初始化
  * @param  void
  * @retval void
  */
  
void Sensor_Init(void)
{
	Relay_Init();
	Infrare_Init();
	Sonic_Init();
	Paint_Init();
//	MPU_Init();
//	while(mpu_dmp_init());
}

/**
  * @brief  继电器初始化
  * @param  void
  * @retval void
  */

void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

    //GPIOC13初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC, GPIO_Pin_15 | GPIO_Pin_14);
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}

/**
  * @brief  超声波测距初始化
  * @param  void
  * @retval void
  */

float Distance = 0;

void Sonic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						//PA1 Trig
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           		//PA0 Echo
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Trig = 0; 			//trig

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	Distance = 500;
}

void Sonic_Trig(void)
{
	Trig=1;
	delay_us(100);
	Trig=0;
		
}

void EXTI0_IRQHandler(void)
{
	static u8 flag_Sta = 0;
	int temp;
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
		if (Echo == 1)
		{
			TIM_SetCounter(TIM4, 0);
			flag_Sta = 1;
			TIM_Cmd(TIM4, ENABLE);
		}
		else
		{
			TIM_Cmd(TIM4, DISABLE);
			if (flag_Sta)
			{
				temp = TIM_GetCounter(TIM4);
				temp = temp * 0.034;
				if (temp > 450)
					temp = 450;
				if (temp < Distance)
					Distance = temp;
			}
			flag_Sta = 0;
		}
	}
}

/**
  * @brief  火焰强度测量
  * @param  void
  * @retval void
  */

int flame;

void Flame_Calculate(void)
{
    float temp;
    temp = Get_Adc_Average(ADC_Channel_10, 3);
    temp = temp * 100 / 4096;
    if (temp < flame)
        flame = (int)temp;
}

/**
  * @brief  雨水测量
  * @param  void
  * @retval void
  */

int rain;

void Rain_Calculate(void)
{
    float temp;
    temp = Get_Adc_Average(ADC_Channel_11, 3);
    temp = temp * 100 / 4096;
    if (temp < rain)
        rain = (int)temp;
}

/**
  * @brief  红外线测量
  * @param  void
  * @retval void
  */

int infrare;

void Infrare_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

    //GPIOC13初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
}

void Infrare_Calculate(void)
{
    int temp;
    temp = Infrare * 100;
    if (temp == 0)
        temp = 1;
    if (temp > infrare)
        infrare = temp;
}

/**
  * @brief  烟雾测量
  * @param  void
  * @retval void
  */

int smoke;

void Smoke_Calculate(void)
{
    float temp;
    temp = Get_Adc_Average(ADC_Channel_12, 3);
    temp = temp * 100 / 4096;
    if (temp > smoke)
        smoke = (int)temp;
}

/**
  * @brief  灯光测量
  * @param  void
  * @retval void
  */

int light;

void Light_Calculate(void)
{
    float temp;
    temp = Get_Adc_Average(ADC_Channel_2, 3);
    temp = temp * 100 / 4096;
    if (temp > light)
        light = (int)temp;
}

int angle = 1750;
u8 change = 0;

float pitch, roll, yaw;

/**
  * @brief  水墨屏初始化
  * @param  void
  * @retval void
  */

u8 refresh = 0;

void Paint_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_5);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


