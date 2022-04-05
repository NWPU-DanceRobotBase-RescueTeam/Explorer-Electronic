#ifndef __SPARKFUN_H
#define __SPARKFUN_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "queue.h"
#include "Angle.h"

#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收
#define ROLL 1
#define PITCH 0
#define YAW 2

typedef union
{
	u8 binary[12];
	float FloatData[3];
} binary2float;


void uart2_init(u32 bound);
void IMU_init(void);
u8 GetSpeed(void);
u8 GetAngle(void);
void UXART2_SendMessage(u8 len ,u16* Message);
#endif
