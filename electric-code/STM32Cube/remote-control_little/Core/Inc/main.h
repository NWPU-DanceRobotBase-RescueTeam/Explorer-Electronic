/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOC
#define KEY7_Pin GPIO_PIN_1
#define KEY7_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_0
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_3
#define KEY4_GPIO_Port GPIOA
#define KEY8_Pin GPIO_PIN_6
#define KEY8_GPIO_Port GPIOA
#define LQTFT_RST_Pin GPIO_PIN_0
#define LQTFT_RST_GPIO_Port GPIOB
#define LQTFT_A0_Pin GPIO_PIN_1
#define LQTFT_A0_GPIO_Port GPIOB
#define LQTFT_SCK_Pin GPIO_PIN_10
#define LQTFT_SCK_GPIO_Port GPIOB
#define LQTFT_CS_Pin GPIO_PIN_12
#define LQTFT_CS_GPIO_Port GPIOB
#define KEY6_Pin GPIO_PIN_13
#define KEY6_GPIO_Port GPIOB
#define LQTFT_SDA_Pin GPIO_PIN_15
#define LQTFT_SDA_GPIO_Port GPIOB
#define KEY5_Pin GPIO_PIN_12
#define KEY5_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_3
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
