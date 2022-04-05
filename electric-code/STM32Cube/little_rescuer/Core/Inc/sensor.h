#ifndef __SENSOR_H
#define __SENSOR_H

#include "main.h"

//超声波SR-04     P3.0    Echo   P3.1  Trig

extern float Distance;

void Sonic_Trig(void);

//火焰传感器
extern int32_t flame;

void Flame_Calculate(void);

//雨滴传感器
extern int32_t rain;

void Rain_Calculate(void);

//红外传感器
extern int32_t infrare;

void Infrare_Calculate(void);

//烟雾传感器
extern int32_t smoke;

void Smoke_Calculate(void);

//光敏电阻
extern int32_t light;

void Light_Calculate(void);


//SG-90舵机
extern int32_t angle;
extern uint8_t change;

extern uint8_t refresh;

void Paint_Init(void);

#define CS  PCout(5)
#define DC  PCout(4)
#define RST  PAout(6)
#define BUSY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键3(WK_UP)

#endif
