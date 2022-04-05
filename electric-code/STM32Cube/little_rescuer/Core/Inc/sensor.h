#ifndef __SENSOR_H
#define __SENSOR_H

#include "main.h"

//������SR-04     P3.0    Echo   P3.1  Trig

extern float Distance;

void Sonic_Trig(void);

//���洫����
extern int32_t flame;

void Flame_Calculate(void);

//��δ�����
extern int32_t rain;

void Rain_Calculate(void);

//���⴫����
extern int32_t infrare;

void Infrare_Calculate(void);

//��������
extern int32_t smoke;

void Smoke_Calculate(void);

//��������
extern int32_t light;

void Light_Calculate(void);


//SG-90���
extern int32_t angle;
extern uint8_t change;

extern uint8_t refresh;

void Paint_Init(void);

#define CS  PCout(5)
#define DC  PCout(4)
#define RST  PAout(6)
#define BUSY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����3(WK_UP)

#endif
