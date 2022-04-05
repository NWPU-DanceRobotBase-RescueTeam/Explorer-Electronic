#ifndef __ARM_H
#define __ARM_H	 

#include "sys.h"

// 电机和舵机的ID
#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4
#define ID5 5
#define ID6 6
#define ID7 7

// 为了与角度匹配的常量
extern const float SERVO_RAD;
extern const float SERVO_RAD1;
extern const float MOTOR_RAD;

void servo_set(u8 id, u16 pwm_re); //舵机设定
void servo_init(void);  //舵机PWM初始化

#endif


