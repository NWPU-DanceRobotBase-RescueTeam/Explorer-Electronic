#include "key.h"
#include "delay.h"
#include "led.h"
#include "tim.h"
#include "OLED.h"

btof base, first_arm, second_arm, small_rotation, small_oscillation, paw_open_close, paw_rotation;
u8 reset, spin;

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

s16 adcxleft, adcyright, adcyleft, adcxright;
u8 mode = 0, lastmode = 0;

/**
  * @brief  改变模式并改变各值
  * @param  void
  * @retval void
  */

void Mode_Change(void)
{
	static u8 i = 0, j = 0;
	lastmode = mode;
	adcxleft = (s16)tadcxleft;
	adcyright = (s16)tadcyright;
	adcyleft = (s16)tadcyleft;
	adcxright = (s16)tadcxright;

	if (mode == 1)
	{
		paw_open_close.Float += (float)(0.01 * adcxleft);
		paw_rotation.Float += (float)(0.01 * adcxright);
		if (paw_open_close.Float >= 2.4f)
			paw_open_close.Float = 2.4;
		else if (paw_open_close.Float <= 1.4f)
			paw_open_close.Float = 1.4;
		if (paw_rotation.Float >= 3.14f)
			paw_rotation.Float = 3.14;
		else if (paw_rotation.Float <= 0.01f)
			paw_rotation.Float = 0.01;
	}
	if (mode == 2)
	{
		small_rotation.Float += (float)(0.01 * adcxleft);
		small_oscillation.Float += (float)(0.01 * adcxright);
		if (small_rotation.Float >= 3.14f)
			small_rotation.Float = 3.14;
		else if (small_rotation.Float <= 0.01f)
			small_rotation.Float = 0.01;
		if (small_oscillation.Float >= 2.9f)
			small_oscillation.Float = 2.9;
		else if (small_oscillation.Float <= 0.785f)
			small_oscillation.Float = 0.785;
	}
	if (mode == 3)
	{
		first_arm.Float += (float)(0.01 * adcyleft);
		second_arm.Float += (float)(0.01 * adcyright);
		if (first_arm.Float >= 3.14f)
			first_arm.Float = 3.14;
		else if (first_arm.Float <= 0.00f)
			first_arm.Float = 0.00;
		if (second_arm.Float >= 3.14f)
			second_arm.Float = 3.14;
		else if (second_arm.Float <= 0.00f)
			second_arm.Float = 0.00;
	}
	if (mode == 4)
	{
		base.Float += (float)(0.01 * adcxright);
		if (base.Float >= 3.14f)
			base.Float = 3.14;
		else if (base.Float <= 0.01f)
			base.Float = 0.01;
	}

	if (key5 == 0)
	{
		if (i++ > 20)
		{
			spin = 1;
		}
	}
	else
		i = 0;

	if (key6 == 0)
	{
		if (key7 == 0)
			if (j++ > 5)
				reset = 1;
	}
	else
		j = 0;

}

/**
  * @brief  根据按键判断当前模式
  * @param  void
  * @retval void
  */

void Key_Scan(void)
{
	if (key1 == 1)
	{
		delay_ms(50);
		if (key1 == 1)
		{
//			send_message[1]=0x38;
			mode = 1;
		}
	}
	else if (key2 == 1)
	{
		delay_ms(50);
		if (key2 == 1)
		{
//			send_message[1]=0x27;
			mode = 2;
		}
	}
	else if (key3 == 1)
	{
		delay_ms(50);
		if (key3 == 1)
		{
//			send_message[1]=0x16;
			mode = 3;
		}
	}
	else if (key4 == 0)
	{
		delay_ms(50);
		if (key4 == 0)
		{
//			send_message[1]=0x05;
			mode = 4;
		}
	}
	else
	{
		delay_ms(50);
		if (key4 == 1 && key1 == 0 && key2 == 0 && key3 == 0)
		{
//			send_message[1]=0x00;
			mode = 0;
		}
	}

}
