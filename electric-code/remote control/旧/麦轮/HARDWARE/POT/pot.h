/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "math.h"
#include "Motor_Control.h"

extern float angle[4];
extern u8 adc_flag[5];

void get_pot_adc(void);//��λ����ѹ�ɼ�
void adc_angle_transform(void);//��λ���Ƕ�ת��
void Subtrack_Rest(void);//���Ĵ���λ
u8 Angle_Position_Control(float Goal_Position,u8 Motor_ID);//���Ĵ�λ�ÿ���

#endif
