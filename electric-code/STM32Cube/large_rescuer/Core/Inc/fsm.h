#ifndef _FSM_H
#define _FSM_H

#include "main.h"

uint8_t Usart_Receivecheck(void);   //��鴮���Ƿ��յ�����
uint8_t FSM(uint8_t len, uint8_t* msg1, uint8_t* mode);     //����״̬��

#endif
