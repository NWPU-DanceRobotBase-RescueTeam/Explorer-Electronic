#include "key.h"


//key1  PC0    key3  PA0      key4  PA3

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC,ENABLE);//使能 GPIOA,GPIOC 时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;// KEY3 KEY4 对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP 对应引脚 
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


