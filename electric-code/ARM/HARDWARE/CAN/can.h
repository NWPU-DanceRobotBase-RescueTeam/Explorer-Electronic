#ifndef __CAN_H
#define __CAN_H	 

#include "sys.h"
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
void CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN��ʼ��

u8 CAN1_Send_Msg(u8* msg, u8 len, u32 id);						//��������

u8 CAN1_Receive_Msg(u8 *buf);							//��������

#define CAN2_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.	
										 							 				    
void CAN2_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN��ʼ��

u8 CAN2_Send_Msg(u8* msg, u8 len, u32 id);						//��������

u8 CAN2_Receive_Msg(u8 *buf);							//��������

#endif

















