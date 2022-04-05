#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 PCout(0)	

void LED_Init(void);  //LED口初始化	
void Wireless_serial_port_Init(void);  //无线串口M0,M1口初始化
	
#endif
