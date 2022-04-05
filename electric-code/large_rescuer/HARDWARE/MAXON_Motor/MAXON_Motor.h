#ifndef _MAXON_MOTOR_H_
#define _MAXON_MOTOR_H_

#include "sys.h"

#define LEFT_Direction PCout(1)
#define RIGHT_Direction PAout(2)

#define LEFT_Speed(n) TIM_SetCompare4(TIM2, n)   //pwmֵ��Ϊ�ٶ�ֵ
#define RIGHT_Speed(n) TIM_SetCompare1(TIM5, n)

extern s16 MAXON_MOTOR[2];  //MAXON����ٶ�

void Maxon_Motor_Init(void);    //���Ĵ���ʼ��
void Maxon_Speed_Control(void);//���Ĵ��ٶȿ���

#endif

