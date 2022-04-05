#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "queue.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			0		//使能（1）/禁止（0）串口1接收
#define RX_LEN_GSM 12

#define SET_BAUDRATE	230400// 波特率设置


#define USE_USART		USART1

extern Queue USART_RX_Queue , USART_TX_Queue;
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint8_t Buffer[2][RX_LEN_GSM];//双缓存
extern u16 USART_RX_STA;         		//接收状态标记	
extern uint8_t finish_flag;
extern u8 now_number,message_pointer;
#define FREE_SPACE (RX_LEN_GSM - message_pointer)
void uart_init(u32 bound);

#endif


