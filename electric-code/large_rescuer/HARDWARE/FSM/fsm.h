#ifndef _FSM_H
#define _FSM_H

#include "sys.h"

u8 Usart_Receivecheck(void);   //检查串口是否收到数据
u8 FSM(u16 len, u8* msg1, u8* mode);     //串口状态机

#endif
