/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "OLED.h"
#include "adc.h"
#include "key.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim5;

/* TIM5 init function */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 8399;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1999;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */
  HAL_TIM_Base_Start_IT(&htim5);
  /* USER CODE END TIM5_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();

    /* TIM5 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();

    /* TIM5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  ∂® ±∆˜5÷–∂œ∑˛ŒÒ∫Ø ˝
  * @param  void
  * @retval void
  */

int16_t limitings(int16_t a, int16_t b)
{
    if (a >= b)
		return b;
    else if (a <= -b)
		return -b;
    else
		return a;
}

uint8_t send_message[8] = { 0xff,0xff,0x02,0x0a,0x0a,0xfe,0x0d,0x0a };
int16_t tadcxleft, tadcyright;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t i;

	if (htim->Instance == htim5.Instance)
	{
		tadcxleft = limitings(-(adc_buf[1] - 2048) / 200, 100);//◊Û“°∏Àx÷·      -10µΩ10
//		adcyleft = limiting((Get_Adc_Average(ADC_Channel_12,5)-2060)/200,100);//◊Û“°∏Ày÷·
//		adcxright = limiting(-(Get_Adc_Average(ADC_Channel_4,5)-2050)/200,100);//”““°∏Àx÷·
		tadcyright = limitings((adc_buf[0] - 2048) / 200, 100);//”““°∏Ày÷·

		Mode_Change();
		Show_Speed();
		if (adcxleft != send_message[3] || adcyright != send_message[4] || send_message[1] != 0xff)
		{
			send_message[3] = adcxleft;
			send_message[4] = adcyright;
			for (uint8_t j = 0; j < 8; j++)
			{
				putchar(send_message[j]);
			}
		}
		if (send_message[1] != 0xff)
			send_message[1] = 0xff;
		i++;
		if (i > 10)
		{
			i = 0;
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		}
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
