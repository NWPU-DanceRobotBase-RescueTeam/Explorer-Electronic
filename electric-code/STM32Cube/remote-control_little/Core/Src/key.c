/*
 * key.c
 *
 *  Created on: 2021��7��18��
 *      Author: ����
 */

#include "key.h"
#include "tim.h"

/**
 * @brief  ��������ģʽ�ı�
 * @param  void
 * @retval void
 */

int16_t adcxleft, adcyright;
uint8_t mode = 0;

void Mode_Change(void)
{
	adcxleft = (int16_t) tadcxleft;
	adcyright = (int16_t) tadcyright;

	if (key1 == 1)    //1��
	{
		adcxleft = (adcxleft + 100) / 2 + 50;
		adcyright = adcyright + 100;
		mode = 1;
	}
	else if (key3 == 1)    //2��
	{
		adcxleft = adcxleft + 100;
		adcyright = adcyright * 2 + 100;
		mode = 2;
	}
	else if (key4 == 1)    //3��
	{
		adcxleft = adcxleft * 2 + 100;
		adcyright = adcyright * 4 + 100;
		mode = 3;
	}
	else           //0��
	{
		adcxleft = 100;           //��ҡ��x��
		adcyright = 100;           //��ҡ��y��
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

