#ifndef __ARM_H
#define __ARM_H	 

#include "sys.h"

// ����Ͷ����ID
#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4
#define ID5 5
#define ID6 6
#define ID7 7

// Ϊ����Ƕ�ƥ��ĳ���
extern const float SERVO_RAD;
extern const float SERVO_RAD1;
extern const float MOTOR_RAD;

void servo_set(u8 id, u16 pwm_re); //����趨
void servo_init(void);  //���PWM��ʼ��

#endif


