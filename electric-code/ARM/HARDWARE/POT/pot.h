/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "sys.h"

extern float first_arm_goal_position;
extern float second_arm_goal_postion;
extern const float SECOND_ARM_START;

void get_pot_adc_angle(void);//电位器电压采集
void Arm_Rest(void);      //二级臂复位
void Angle_Position_Control(u8 Motor_ID);//二级臂位置控制

#endif
