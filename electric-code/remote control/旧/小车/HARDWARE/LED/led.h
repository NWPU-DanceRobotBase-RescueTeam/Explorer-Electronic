#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PBout(3)	// DS0
#define LED1 PBout(4)	// DS0
#define LED2 PBout(5)	// DS0
#define LED3 PBout(6)	// DS0


void LED_Init(void);//��ʼ��		 
void Wireless_serial_port_Init(void);//���ߴ��ڳ�ʼ��
#endif
