#ifndef __DMA_H
#define __DMA_H 	
#include "sys.h"


extern u8 ReceiveBuff[16];		//接收缓存区
void DMA_Config_A(void);		//串口2接收使用，数据传输量16或12
void DMA_Config_B(u32 mar);		//串口3发送使用，mar为发送数据存储器地址
void DMA_Config_C(u32 mar);		//串口1发送使用（DeBug）
void DMA_A_Enable(void);			//完成一次串口2 DMA接收
void DMA_B_Enable(void);			//完成一次串口3 DMA发送
	
void DMA_C_Enable(void);			//完成一次串口1 DMA发送
void AngleSend(volatile u8* data);	//DMA舵机角度发送
#endif

