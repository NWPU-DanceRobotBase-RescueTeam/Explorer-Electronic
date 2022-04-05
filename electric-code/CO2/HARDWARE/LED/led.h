#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口定义
#define LED0 PAout(15)	// DS0	 
#define WAK PDout(2)

void LED_Init(void);//初始化	
void Wireless_serial_port_Init(void);  //无线串口M0,M1口初始化
void CJ811_Init(void);

#endif
