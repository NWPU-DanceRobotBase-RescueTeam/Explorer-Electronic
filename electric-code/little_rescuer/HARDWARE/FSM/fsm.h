#ifndef _FSM_H
#define _FSM_H

#include "sys.h"

u8 Usart_Receivecheck(void);
u8 FSM(u16 len,u8* msg1,u8* mode);

#endif
