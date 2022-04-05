#ifndef _MAXON_MOTOR_H_
#define _MAXON_MOTOR_H_

#include "main.h"

#define LEFT_Speed(n) __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,n) //pwm值即为速度值
#define RIGHT_Speed(n) __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,n)

extern int16_t MAXON_MOTOR[2];  //MAXON电机速度

void Maxon_Speed_Control(void);//主履带速度控制

#endif

