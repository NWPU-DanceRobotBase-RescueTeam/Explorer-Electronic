/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "main.h"

// �����Ĵ���λ�Ƕ�
#define Left_Front 198
#define Left_Rear 80
#define Right_Front 92
#define Right_Rear 200

#define Max_Speed 6000

void get_pot_adc_angle(uint8_t ID); // ��λ����ѹ�ɼ�
void Subtrack_Rest(void);    // ���Ĵ���λ
void Angle_Position_Control(void);// ���Ĵ�λ�ÿ���

#endif
