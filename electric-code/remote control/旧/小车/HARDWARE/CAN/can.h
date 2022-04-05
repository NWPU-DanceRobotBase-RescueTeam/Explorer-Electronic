/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H	 

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "pot.h"
#include "string.h"
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��

u8 CAN1_Send_Msg(u8* msg,u8 len,uint32_t id);						//��������

u8 CAN1_Receive_Msg(u8 *buf);							//��������



#define CAN2_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��

u8 CAN2_Send_Msg(u8* msg,u8 len,uint32_t id);						//��������

u8 CAN2_Receive_Msg(u8 *buf);							//��������
#endif



