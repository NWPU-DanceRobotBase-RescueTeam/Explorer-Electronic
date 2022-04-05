#ifndef _MAXON_MOTOR_H_
#define _MAXON_MOTOR_H_

#include "sys.h"

#define LEFT_Direction PCout(1)
#define RIGHT_Direction PAout(2)

#define LEFT_Speed(n) TIM_SetCompare4(TIM2, n)   //pwm值即为速度值
#define RIGHT_Speed(n) TIM_SetCompare1(TIM5, n)

extern s16 MAXON_MOTOR[2];  //MAXON电机速度

void Maxon_Motor_Init(void);    //主履带初始化
void Maxon_Speed_Control(void);//主履带速度控制

#endif

