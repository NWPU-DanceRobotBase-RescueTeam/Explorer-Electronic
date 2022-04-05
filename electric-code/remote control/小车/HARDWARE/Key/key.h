#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

#define key1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) //PE4
#define key3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) //PE3
#define key4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) //PE2
#define key5 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//×ó°´Å¥
#define key6 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//ÓÒ°´Å¥
#define key7 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//×óÒ¡¸Ë°´¼ü
#define key8 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//×óÒ¡¸Ë°´¼ü

void KEY_Init(void);
void Mode_Change(void);
void Key_Scan(void);

extern s16 adcxleft, adcyright;
extern u8 mode;

#endif

