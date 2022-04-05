#ifndef _FSM_H
#define _FSM_H

#include "main.h"

uint8_t Usart_Receivecheck(void);   //检查串口是否收到数据
uint8_t FSM(uint8_t len, uint8_t* msg1, uint8_t* mode);     //串口状态机

#endif
