#include "sensor.h"

/**
 * @brief  ����������ʼ��
 * @param  void
 * @retval void
 */

float Distance = 500;

void Sonic_Trig(void)
{
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint8_t flag_Sta = 0;
	int32_t temp;
	if (GPIO_Pin == GPIO_PIN_0)
	{
		if (HAL_GPIO_ReadPin(Echo_GPIO_Port, Echo_Pin) == 1)
		{
			__HAL_TIM_SET_COUNTER(&htim4, 0);
			flag_Sta = 1;
			HAL_TIM_Base_Start(&htim4);
		}
		else
		{
			HAL_TIM_Base_Stop(&htim4);
			if (flag_Sta)
			{
				temp = __HAL_TIM_GET_COUNTER(&htim4);
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
 * @brief  ����ǿ�Ȳ���
 * @param  void
 * @retval void
 */

int32_t flame;

void Flame_Calculate(void)
{
	int32_t temp = adc_buf[0] * 100 / 4096;
	if (temp < flame)
		flame = (int32_t) temp;
}

/**
 * @brief  ��ˮ����
 * @param  void
 * @retval void
 */

int32_t rain;

void Rain_Calculate(void)
{
	int32_t temp = adc_buf[1] * 100 / 4096;
	if (temp < rain)
		rain = (int32_t) temp;
}

/**
 * @brief  �����߲���
 * @param  void
 * @retval void
 */

int32_t infrare;

void Infrare_Calculate(void)
{
	uint8_t temp = HAL_GPIO_ReadPin(INFRARE_GPIO_Port, INFRARE_Pin) * 100;
	if (temp == 0)
		temp = 1;
	if (temp > infrare)
		infrare = temp;
}

/**
 * @brief  �������
 * @param  void
 * @retval void
 */

int32_t smoke;

void Smoke_Calculate(void)
{
	int32_t temp = adc_buf[2] * 100 / 4096;
	if (temp > smoke)
		smoke = (int32_t) temp;
}

/**
 * @brief  �ƹ����
 * @param  void
 * @retval void
 */

int32_t light;

void Light_Calculate(void)
{
	int32_t temp = adc_buf[3] * 100 / 4096;
	if (temp > light)
		light = (int32_t) temp;
}

int32_t angle = 1750;
uint8_t change = 0;

/**
 * @brief  ˮī����ʼ��
 * @param  void
 * @retval void
 */

uint8_t refresh = 0;

//void Paint_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	GPIO_SetBits(GPIOC, GPIO_Pin_5);
//	GPIO_SetBits(GPIOA, GPIO_Pin_6);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//}

