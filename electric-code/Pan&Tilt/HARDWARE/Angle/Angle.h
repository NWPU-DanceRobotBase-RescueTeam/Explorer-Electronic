#ifndef __Angle_H
#define __Angle_H
#include "queue.h"


typedef struct
{
	float zero;  //当前值
	float fir;   //上一次值
	float sec;   //上一次的上一次值
} VALUE;

typedef struct
{
	float Kp;
	float Kd;
	float Ki;
} PID;

typedef struct
{
	VALUE val;   //当前角度偏差值
	PID pid;     //PID参数
	float speed;  //角速度值
	float realval;  //累计出的真实值（参数调不好会有误差）
} ANGLE;

extern float Angle[3];    //角度数值
extern ANGLE RollAngle,PitchAngle;   //处理翻滚角和俯仰角数据的结构体

void Angle_Init(void);  //角度值结构体的初始化
void AngleControl(float value, ANGLE *Angle); //角度数据处理函数
void AngleUpdate(void);
void AngleIsFinite(void);		
#endif
