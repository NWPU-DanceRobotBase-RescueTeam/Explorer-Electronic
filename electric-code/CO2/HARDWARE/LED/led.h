#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED�˿ڶ���
#define LED0 PAout(15)	// DS0	 
#define WAK PDout(2)

void LED_Init(void);//��ʼ��	
void Wireless_serial_port_Init(void);  //���ߴ���M0,M1�ڳ�ʼ��
void CJ811_Init(void);

#endif
