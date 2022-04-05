#ifndef __GIM8008__H
#define __GIM8008__H

#include "sys.h"

#define CMD_MOTOR_MODE      0x01
#define CMD_RESET_MODE      0x02
#define CMD_ZERO_POSITION   0x03

#define P_MIN -95.5f    // Radians
#define P_MAX 95.5f        
#define V_MIN -45.0f    // Rad/s
#define V_MAX 45.0f
#define KP_MIN 0.0f     // N-m/rad
#define KP_MAX 500.0f
#define KD_MIN 0.0f     // N-m/rad/s
#define KD_MAX 5.0f
#define T_MIN -18.0f
#define T_MAX 18.0f

#define GIM_Kp 15
#define GIM_Kd 0.5

void Set_8008_Mode(u8 mode);  //�����������
void Send_8008_Control(float f_p, float f_v, float f_kp, float f_kd, float f_t);//���������Ϣ
void MotorControl_Start(void);//�����ʼ��
//void MotorControl_Stop(void);//���ʧ��

#endif
