#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

#define key1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) 
#define key2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) 
#define key3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) 
#define key4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ť
#define key5 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//�Ұ�ť
#define key6 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//��ҡ�˰���
#define key7 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ҡ�˰���

typedef union
{
	u8 Binary[4];
	float Float;
} btof;

extern s16 adcxleft, adcyright, adcyleft, adcxright;
extern u8 mode, lastmode;
extern btof base, first_arm, second_arm, small_rotation, small_oscillation, paw_open_close, paw_rotation;
extern u8 reset, spin;

void KEY_Init(void);
void Mode_Change(void);
void Key_Scan(void);

#endif

