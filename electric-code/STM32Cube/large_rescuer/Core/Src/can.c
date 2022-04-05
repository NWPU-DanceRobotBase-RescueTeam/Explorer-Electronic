/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
	CAN1Filter_Config();
  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_7TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_6TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
	CAN2Filter_Config();
  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CAN1Filter_Config(void)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterBank = 0;                       //CAN��������ţ���Χ0-27
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;   //CAN������ģʽ������ģʽ���б�ģʽ
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;  //CAN�������߶ȣ�16λ��32λ
	sFilterConfig.FilterIdHigh = (0x01e0 >> 5) << 5;
	sFilterConfig.FilterIdLow = (0x0be0 >> 5) << 5;
	sFilterConfig.FilterMaskIdHigh = (0x03e0 >> 5) << 5;
	sFilterConfig.FilterMaskIdLow = (0x04e0 >> 5) << 5;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;				//����ͨ����������ƥ��󣬴洢���ĸ�FIFO
	sFilterConfig.FilterActivation = ENABLE;    		//���������
	sFilterConfig.SlaveStartFilterBank = 0;

	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN2Filter_Config(void)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterBank = 14;                       //CAN��������ţ���Χ0-27
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;   //CAN������ģʽ������ģʽ���б�ģʽ
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;  //CAN�������߶ȣ�16λ��32λ
	sFilterConfig.FilterIdHigh = 0x0201 << 5;
	sFilterConfig.FilterIdLow = 0x0202 << 5;
	sFilterConfig.FilterMaskIdHigh = 0x0203 << 5;
	sFilterConfig.FilterMaskIdLow = 0x0204 << 5;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;				//����ͨ����������ƥ��󣬴洢���ĸ�FIFO
	sFilterConfig.FilterActivation = ENABLE;    		//���������
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_Start(&hcan2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan == &hcan1)
	{
		CAN_RxHeaderTypeDef RxMessage;
		uint8_t data[8];

		if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMessage, data) != HAL_OK)
		{
			Error_Handler();
		}

		static float Goal_Speed = 0;
		static float Goal_Position = 0;

		switch (RxMessage.StdId)
		{
			// ����������Ϣ
			case 0x01e0 >> 5:
			{
				memcpy(&Goal_Speed, data, 4);
				Goal_Speed *= SPEED_MULTIPLE;
				MAXON_MOTOR[0] = (int16_t) Goal_Speed;
				printf("�����Ĵ��趨�ٶ�Ϊ:%d\r\n", MAXON_MOTOR[0]);

				memcpy(&Goal_Speed, data + 4, 4);
				Goal_Speed *= SPEED_MULTIPLE;
				MAXON_MOTOR[1] = -(int16_t) Goal_Speed;
				printf("�����Ĵ��趨�ٶ�Ϊ:%d\r\n", -MAXON_MOTOR[1]);

				break;
			}
			case 0x03e0 >> 5:
			{
				memcpy(&Goal_Position, data, 4);
				Goal_Position *= 60;
				M3508[0].PID.Goal_Position = Left_Front - (int32_t) Goal_Position;
				if (M3508[0].PID.Goal_Position > Left_Front)
					M3508[0].PID.Goal_Position = Left_Front;
				else if (M3508[0].PID.Goal_Position < 40)
					M3508[0].PID.Goal_Position = 40;
				M3508[0].Stop_Flag = 0;
				printf("��ǰ���Ĵ��趨�Ƕ�Ϊ:%d\r\n", M3508[0].PID.Goal_Position);

				memcpy(&Goal_Position, data + 4, 4);
				Goal_Position *= 60;
				M3508[2].PID.Goal_Position = Right_Front + (int32_t) Goal_Position;
				if (M3508[2].PID.Goal_Position < Right_Front)
					M3508[2].PID.Goal_Position = Right_Front;
				else if (M3508[2].PID.Goal_Position > 240)
					M3508[2].PID.Goal_Position = 240;
				M3508[2].Stop_Flag = 0;
				printf("��ǰ���Ĵ��趨�Ƕ�Ϊ:%d\r\n", M3508[2].PID.Goal_Position);

				break;
			}
			case 0x04e0 >> 5:
			{
				memcpy(&Goal_Position, data, 4);
				Goal_Position *= 60;
				M3508[1].PID.Goal_Position = Left_Rear + (int32_t) Goal_Position;
				if (M3508[1].PID.Goal_Position < Left_Rear)
					M3508[1].PID.Goal_Position = Left_Rear;
				else if (M3508[1].PID.Goal_Position > 240)
					M3508[1].PID.Goal_Position = 240;
				M3508[1].Stop_Flag = 0;
				printf("����Ĵ��趨�Ƕ�Ϊ:%d\r\n", M3508[1].PID.Goal_Position);

				memcpy(&Goal_Position, data + 4, 4);
				Goal_Position *= 60;
				M3508[3].PID.Goal_Position = Right_Rear - (int32_t) Goal_Position;
				if (M3508[3].PID.Goal_Position > Right_Rear)
					M3508[3].PID.Goal_Position = Right_Rear;
				else if (M3508[3].PID.Goal_Position < 40)
					M3508[3].PID.Goal_Position = 40;
				M3508[3].Stop_Flag = 0;
				printf("�Һ��Ĵ��趨�Ƕ�Ϊ:%d\r\n", M3508[3].PID.Goal_Position);

				break;
			}
				//		case 0x0be0 >> 5 :
				//		{
				//			memcpy(&shut,data,4);
				//			if(fabs(shut-0)>=0.1&&a>=20)
				//			{
				//				relay_OFF();
				//				a = 0;
				//				times_numb1 =0;
				//			}
				//			break;
				//		}
		}
	}
}

void CAN1_Send_Msg(uint8_t *data, uint32_t id)
{
	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = id;	 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId = 0x00;	 // ������չ��ʾ����29λ��
	TxMessage.IDE = CAN_ID_STD;		  // ʹ����չ��ʶ��
	TxMessage.RTR = CAN_RTR_DATA;		  // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC = 8;							 // ������֡��Ϣ
	TxMessage.TransmitGlobalTime = DISABLE;

	uint32_t TxMailbox;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if (hcan == &hcan2)
	{
		CAN_RxHeaderTypeDef RxMessage;
		uint8_t data[8];

		if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxMessage, data) != HAL_OK)
		{
			Error_Handler();
		}

		switch (RxMessage.StdId)
		{
			case 0x0201:
			{
				for (uint8_t i = 0; i < 8; ++i)
				{
					//����ԭʼ��Ϣ
					M3508[0].Original_Data[i] = data[i];
				}
				//������Ϣ��ת��
				Data_Convert(0);

				break;
			}
			case 0x0202:
			{
				for (uint8_t i = 0; i < 8; ++i)
				{
					//����ԭʼ��Ϣ
					M3508[1].Original_Data[i] = data[i];
				}
				//������Ϣ��ת��
				Data_Convert(1);

				break;
			}
			case 0x0203:
			{
				for (uint8_t i = 0; i < 8; ++i)
				{
					//����ԭʼ��Ϣ
					M3508[2].Original_Data[i] = data[i];
				}
				//������Ϣ��ת��
				Data_Convert(2);

				break;
			}
			case 0x0204:
			{
				for (uint8_t i = 0; i < 8; ++i)
				{
					//����ԭʼ��Ϣ
					M3508[3].Original_Data[i] = data[i];
				}
				//������Ϣ��ת��
				Data_Convert(3);

				break;
			}
		}
	}
}

void CAN2_Send_Msg(uint8_t *msg, uint32_t id)
{
	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = id;	 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId = 0x00;	 // ������չ��ʾ����29λ��
	TxMessage.IDE = CAN_ID_STD;		  // ʹ����չ��ʶ��
	TxMessage.RTR = CAN_RTR_DATA;		  // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC = 8;							 // ������֡��Ϣ
	TxMessage.TransmitGlobalTime = DISABLE;

	uint8_t data[8];
	for (uint8_t i = 0; i < 8; i++)
		data[i] = msg[i];

	uint32_t TxMailbox;
	if (HAL_CAN_AddTxMessage(&hcan2, &TxMessage, data, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
