#include "key.h"
#include "delay.h"
#include "led.h"
#include "tim.h"
#include "OLED.h"

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

s16 adcxleft, adcyright, fulvdai[4], adcyleft;
u8 mode = 0, fuwei = 0, lastmode = 0;

s16 limiting(s16 a, s16 b, s16 c)
{
    if (a <= b) 
		return b;
    else if (a >= c)
		return c;
    else 
		return a;
}

void Mode_Change(void)
{
	static s8 i;
	lastmode = mode;
	adcxleft = (s16)tadcxleft;
	adcyright = (s16)tadcyright;
	adcyleft = (s16)tadcyleft;
	if (key7 == 0)
	{
		if (key8 == 0)
		{
			i++;
			if (i > 5)
			{
				for (i = 0; i < 4; i++)
					fulvdai[i] = 0;
				Show_Reset();
				i = 0;
			}
		}
		else
			i = 0;
	}
	else
		i = 0;
	if (key5 == 0)
	{
		adcyleft = limiting(adcyleft, -5, 5);
		adcyright = limiting(adcyright, -5, 5);
		
		fulvdai[0] += adcyleft;
		fulvdai[2] += adcyright;
		
		fulvdai[0] = limiting(fulvdai[0], 0, 125);
		fulvdai[2] = limiting(fulvdai[2], 0, 125);
		
		mode = 4;
	}
	else if (key6 == 0)
	{
		adcyleft = limiting(adcyleft, -5, 5);
		adcyright = limiting(adcyright, -5, 5);
		
		fulvdai[1] += adcyleft;
		fulvdai[3] += adcyright;
		
		fulvdai[1] = limiting(fulvdai[1], 0, 125);
		fulvdai[3] = limiting(fulvdai[3], 0, 125);
		
		mode = 5;
	}
	else if (key1 == 1)    //1档
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

/**
  * @brief  根据按键判断当前模式
  * @param  void
  * @retval void
  */

void Key_Scan(void)
{
//	if(key5==0)
//	{
//		delay_ms(50);
//		if(key5==0)
//		{
//			delay_ms(50);
//			send_message[1]=0x01;
//			delay_ms(50);
//		}
//	}
}

