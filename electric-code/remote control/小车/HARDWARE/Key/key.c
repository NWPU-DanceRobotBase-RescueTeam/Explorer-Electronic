#include "key.h"
#include "tim.h"
#include "delay.h"
#include "led.h"

/**
  * @brief  开关初始化
  * @param  void
  * @retval void
  */
  
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//使能 GPIOA,GPIOC 时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3;// KEY3 KEY4 对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP 对应引脚 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  根据现在模式改变
  * @param  void
  * @retval void
  */

s16 adcxleft, adcyright;
u8 mode = 0;

void Mode_Change(void)
{
	adcxleft = (s16)tadcxleft;
	adcyright = (s16)tadcyright;

	if (key1 == 1)    //1档
	{
		adcxleft = (adcxleft + 100) / 2 + 50;
		adcyright = adcyright + 100;
		mode = 1;
	}
	else if (key3 == 1)    //2档
	{
		adcxleft = adcxleft + 100;
		adcyright = adcyright * 2 + 100;
		mode = 2;
	}
	else if (key4 == 1)    //3档
	{
		adcxleft = adcxleft * 2 + 100;
		adcyright = adcyright * 4 + 100;
		mode = 3;
	}
	else           //0档
	{
		adcxleft = 100;//左摇杆x轴
		adcyright = 100;//右摇杆y轴
		mode = 0;
	}
}

void Key_Scan(void)
{
	if (key5 == 0)
	{
		delay_ms(50);
		if (key5 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x01;
			delay_ms(50);
		}
	}
	else if (key6 == 0)
	{
		delay_ms(50);
		if (key6 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x13;
			delay_ms(50);
		}
	}
	else if (key7 == 0)
	{
		delay_ms(50);
		if (key7 == 0)
		{
			delay_ms(50);
			send_message[1] = 0x25;
			delay_ms(50);
			LED1 = ~LED1;


		}
	}
	else if (key8 == 0)
	{
		delay_ms(50);
		if (key8 == 0)
		{
			LED2 = ~LED2;
			delay_ms(50);
			send_message[1] = 0x37;
			delay_ms(50);

		}
	}
}
