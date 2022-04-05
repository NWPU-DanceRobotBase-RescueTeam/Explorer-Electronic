#ifndef __Angle_H
#define __Angle_H
#include "queue.h"


typedef struct
{
	float zero;  //��ǰֵ
	float fir;   //��һ��ֵ
	float sec;   //��һ�ε���һ��ֵ
} VALUE;

typedef struct
{
	float Kp;
	float Kd;
	float Ki;
} PID;

typedef struct
{
	VALUE val;   //��ǰ�Ƕ�ƫ��ֵ
	PID pid;     //PID����
	float speed;  //���ٶ�ֵ
	float realval;  //�ۼƳ�����ʵֵ�����������û�����
} ANGLE;

extern float Angle[3];    //�Ƕ���ֵ
extern ANGLE RollAngle,PitchAngle;   //�������Ǻ͸��������ݵĽṹ��

void Angle_Init(void);  //�Ƕ�ֵ�ṹ��ĳ�ʼ��
void AngleControl(float value, ANGLE *Angle); //�Ƕ����ݴ�����
void AngleUpdate(void);
void AngleIsFinite(void);		
#endif
