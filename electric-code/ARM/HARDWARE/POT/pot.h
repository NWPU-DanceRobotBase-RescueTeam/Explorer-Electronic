/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "sys.h"

extern float first_arm_goal_position;
extern float second_arm_goal_postion;
extern const float SECOND_ARM_START;

void get_pot_adc_angle(void);//��λ����ѹ�ɼ�
void Arm_Rest(void);      //�����۸�λ
void Angle_Position_Control(u8 Motor_ID);//������λ�ÿ���

#endif
