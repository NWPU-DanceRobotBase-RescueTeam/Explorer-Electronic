#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PCout(0)	

void LED_Init(void);  //LED�ڳ�ʼ��	
void Wireless_serial_port_Init(void);  //���ߴ���M0,M1�ڳ�ʼ��
	
#endif
