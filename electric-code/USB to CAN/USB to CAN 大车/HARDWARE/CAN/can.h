/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _BSP_CAN_H
#define _BSP_CAN_H	 

#include "sys.h"


#define Set_ID	(uint32_t)0x002//��׼IDҪС��11λ
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
	
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��


uint8_t CAN_SetMsg(void); 							//���ñ���

#endif



