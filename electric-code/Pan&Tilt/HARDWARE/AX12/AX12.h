#ifndef __AX12_H
#define __AX12_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口3接收
#define PB12 PBout(12)	// PB12
#define PB13 PBout(13)	// PB13

void uart3_init(u32 bound);
void AX12_Init(void);
u8 axModifyID(u8 ID, u8 trgID);

#endif
