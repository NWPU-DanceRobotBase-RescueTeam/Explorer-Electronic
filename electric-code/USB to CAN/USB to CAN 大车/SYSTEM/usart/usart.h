#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "queue.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define RX_LEN_GSM 12

#define SET_BAUDRATE	230400// ����������


#define USE_USART		USART1

extern Queue USART_RX_Queue , USART_TX_Queue;
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint8_t Buffer[2][RX_LEN_GSM];//˫����
extern u16 USART_RX_STA;         		//����״̬���	
extern uint8_t finish_flag;
extern u8 now_number,message_pointer;
#define FREE_SPACE (RX_LEN_GSM - message_pointer)
void uart_init(u32 bound);

#endif


