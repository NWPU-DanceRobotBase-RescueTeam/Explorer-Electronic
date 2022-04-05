#include "P_bsp.h"

/**
  * @brief  初始函数
  * @param  None
  * @retval None
  */
void Bsp_init(void)
{
	delay_init(168);
	NVIC_Group_Init();
	uart_init(SET_BAUDRATE);
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);
	LED_Init();
}

/**
  * @brief 	中断组初始函数
  * @param  None
  * @retval None
  */
void NVIC_Group_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
