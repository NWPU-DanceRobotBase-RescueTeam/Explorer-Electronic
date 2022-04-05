#include "P_bsp.h"

/**
  * @brief  ��ʼ����
  * @param  None
  * @retval None
  */
void Bsp_init(void)
{
	delay_init();
	NVIC_Group_Init();
	USART_Cofig();
	CAN_Config();
	
}

/**
  * @brief 	�ж����ʼ����
  * @param  None
  * @retval None
  */
void NVIC_Group_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}
