#ifndef __LED_H
#define __LED_H

#include "sys.h"

//LED端口定义
#define LED0 PBout(3)	
#define LED1 PBout(4)	
#define LED2 PBout(5)	
#define LED3 PBout(6)	

void LED_Init(void);//初始化

#endif
