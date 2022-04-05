#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0 PCout(13)	//LED

#define RELAY PCout(5)  //继电器

void LED_Init(void);   //LED灯初始化
void Wireless_serial_port_Init(void);  //无线串口模式设置初始化
void RELAY_Init(void);   //继电器口初始化

#endif
