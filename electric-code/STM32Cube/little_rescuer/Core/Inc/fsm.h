#ifndef _FSM_H
#define _FSM_H

#include "main.h"

uint8_t Usart_Receivecheck(void);
uint8_t FSM(uint16_t len,uint8_t* msg1,uint8_t* mode);

#endif
