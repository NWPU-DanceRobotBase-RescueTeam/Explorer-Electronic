#include "led.h" 

/**
  * @brief  LED口初始化
  * @param  void
  * @retval void
  */

void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOC时钟

	//GPIOC0初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}

/**
  * @brief  无线串口M0,M1口初始化
  * @param  void
  * @retval void
  */

void Wireless_serial_port_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

	//GPIOB4,5,初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_5);
}



