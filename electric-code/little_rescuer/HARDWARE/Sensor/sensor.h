#ifndef __SENSOR_H
#define __SENSOR_H

#include "sys.h"

void Sensor_Init(void);

void Relay_Init(void);

#define headlight PCout(15)
#define buzzer PCout(14)
#define lamp PBout(9)


//超声波SR-04     P3.0    Echo   P3.1  Trig
#define Trig PAout(1)
#define Echo GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

extern float Distance;

void Sonic_Init(void);
void Sonic_Trig(void);


//火焰传感器        
extern int flame;

void Flame_Calculate(void);


//雨滴传感器
extern int rain;

void Rain_Calculate(void);


//红外传感器
extern int infrare;
#define Infrare GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
void Infrare_Init(void);
void Infrare_Calculate(void);


//烟雾传感器
extern int smoke;

void Smoke_Calculate(void);


//光敏电阻
extern int light;

void Light_Calculate(void);


//SG-90舵机
extern int angle;
extern u8 change;

extern float pitch, roll, yaw;

extern u8 refresh;

void Paint_Init(void);

#define CS  PCout(5)
#define DC  PCout(4)
#define RST  PAout(6)
#define BUSY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键3(WK_UP) 

#endif
