#ifndef __CAN_H
#define __CAN_H	 

#include "sys.h"	     
	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//CAN1接收RX0中断使能    0,不使能;1,使能.								    
										 							 				    
void CAN1_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN1初始化

u8 CAN1_Send_Msg(u8* msg, u8 len, u32 id);				//CAN1发送数据

u8 CAN1_Receive_Msg(u8 *buf);							//CAN2接收数据


#define SPEED_MULTIPLE 750       //速度档位调整倍数


#define CAN2_RX0_INT_ENABLE	1		//CAN2接收RX0中断使能    0,不使能;1,使能.								    
										 							 				    
void CAN2_Mode_Init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN1初始化

u8 CAN2_Send_Msg(u8* msg, u8 len, u32 id);				//CAN2发送数据

u8 CAN2_Receive_Msg(u8 *buf);							//CAN2接收数据

#endif

















