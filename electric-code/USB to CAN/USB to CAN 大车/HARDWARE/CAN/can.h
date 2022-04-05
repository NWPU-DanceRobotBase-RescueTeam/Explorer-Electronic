/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H	 

#include "sys.h"


#define Set_ID	(uint32_t)0x002//标准ID要小于11位
#define Main_Track	Set_ID | 0x000
#define Side_Track	Set_ID | 0x001

extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;

typedef enum 
{ 
	CAN_START,
	CAN_DATASTD1,
	CAN_DATASTD2,
	CAN_DATA,
	CAN_CHECK} CAN_STA; 
	
	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化


uint8_t CAN_SetMsg(void); 							//设置报文

#endif



