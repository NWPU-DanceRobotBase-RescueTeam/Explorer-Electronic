/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H	 

#include "sys.h"
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//CAN1����RX0�ж�ʹ��    0,��ʹ��;1,ʹ��.								    
										 							 				    
void CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN1��ʼ��

u8 CAN1_Send_Msg(u8* msg, u8 len, u32 id);				//CAN1��������

u8 CAN1_Receive_Msg(u8 *buf);							//CAN2��������


#define CAN2_RX0_INT_ENABLE	1		//CAN2����RX0�ж�ʹ��    0,��ʹ��;1,ʹ��.								    
										 							 				    
void CAN2_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN1��ʼ��

u8 CAN2_Send_Msg(u8* msg, u8 len, u32 id);				//CAN2��������

u8 CAN2_Receive_Msg(u8 *buf);							//CAN2��������

#endif



