#include "led.h"

/**
  * @brief  LED灯初始化
  * @param  void
  * @retval void
  */
 
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

	//GPIOC13,初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//LED0对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO

	GPIO_ResetBits(GPIOC, GPIO_Pin_13);//GPIOC13,设置高，灯灭
}

/**
  * @brief  无线串口模式设置初始化,将M0,M1置为0,即正常模式
  * @param  void
  * @retval void
  */

void Wireless_serial_port_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOC13,初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	// M0,M1=0
	GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);
}

