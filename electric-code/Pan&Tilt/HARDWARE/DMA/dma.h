#ifndef __DMA_H
#define __DMA_H 	
#include "sys.h"


extern u8 ReceiveBuff[16];		//���ջ�����
void DMA_Config_A(void);		//����2����ʹ�ã����ݴ�����16��12
void DMA_Config_B(u32 mar);		//����3����ʹ�ã�marΪ�������ݴ洢����ַ
void DMA_Config_C(u32 mar);		//����1����ʹ�ã�DeBug��
void DMA_A_Enable(void);			//���һ�δ���2 DMA����
void DMA_B_Enable(void);			//���һ�δ���3 DMA����
	
void DMA_C_Enable(void);			//���һ�δ���1 DMA����
void AngleSend(volatile u8* data);	//DMA����Ƕȷ���
#endif

