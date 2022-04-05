#ifndef _MAXON_MOTOR_H_
#define _MAXON_MOTOR_H_

#include "main.h"

#define LEFT_Speed(n) __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,n) //pwmֵ��Ϊ�ٶ�ֵ
#define RIGHT_Speed(n) __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,n)

extern int16_t MAXON_MOTOR[2];  //MAXON����ٶ�

void Maxon_Speed_Control(void);//���Ĵ��ٶȿ���

#endif

