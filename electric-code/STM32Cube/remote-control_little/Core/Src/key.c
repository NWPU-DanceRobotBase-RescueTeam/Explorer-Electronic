/*
 * key.c
 *
 *  Created on: 2021年7月18日
 *      Author: 神祖
 */

#include "key.h"
#include "tim.h"

/**
 * @brief  根据现在模式改变
 * @param  void
 * @retval void
 */

int16_t adcxleft, adcyright;
uint8_t mode = 0;

void Mode_Change(void)
{
	adcxleft = (int16_t) tadcxleft;
	adcyright = (int16_t) tadcyright;

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
		adcxleft = 100;           //左摇杆x轴
		adcyright = 100;           //右摇杆y轴
		mode = 0;
	}
}

void Key_Scan(void)
{
	if (key5 == 0)
	{
		HAL_Delay(50);
		if (key5 == 0)
		{
			HAL_Delay(50);
			send_message[1] = 0x01;
			HAL_Delay(50);
		}
	}
	else if (key6 == 0)
	{
		HAL_Delay(50);
		if (key6 == 0)
		{
			HAL_Delay(50);
			send_message[1] = 0x13;
			HAL_Delay(50);
		}
	}
	else if (key7 == 0)
	{
		HAL_Delay(50);
		if (key7 == 0)
		{
			HAL_Delay(50);
			send_message[1] = 0x25;
			HAL_Delay(50);
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

		}
	}
	else if (key8 == 0)
	{
		HAL_Delay(50);
		if (key8 == 0)
		{
			HAL_Delay(50);
			send_message[1] = 0x37;
			HAL_Delay(50);
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		}
	}
}

