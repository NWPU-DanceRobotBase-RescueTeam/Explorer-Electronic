#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0 PCout(13)	//LED

#define RELAY PCout(5)  //�̵���

void LED_Init(void);   //LED�Ƴ�ʼ��
void Wireless_serial_port_Init(void);  //���ߴ���ģʽ���ó�ʼ��
void RELAY_Init(void);   //�̵����ڳ�ʼ��

#endif
