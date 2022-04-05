#include "led.h" 

////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PC13为输出口，并使能这个口的时钟   
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOC时钟

  //GPIOC13,初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOA,GPIO_Pin_15);//GPIOC13,设置高，灯灭

}








