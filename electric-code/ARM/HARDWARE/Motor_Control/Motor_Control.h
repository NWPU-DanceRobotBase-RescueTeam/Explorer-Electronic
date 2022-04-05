/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include "sys.h"

struct PID_T
{
	double Kp;
	double Ki;
	double Kd;
	double Kp2;
	double Ki2;
	double Kd2;
	float Last_Error;
	float error;
	float Next_Error;
	float Error_Sum;
	// 电机目标速度
	s16 Goal_Speed;
};
//typedef struct
//{
//	double a;
//	double last_output;
//}FILTER1_HANDLE, *P_FILTER1_HANDLE;
struct MOTOR_T
{
	// 二级臂角度
	float Current_angle;
	// 机械角度
	int32_t Angle;
	// 电机速度
	int32_t Speed;
	// 电机力矩
	int32_t Moment;
	// 原始速度
	u8 Original_Data[8];
	// 电机控制电流
	int16_t Control_Current;
//	//电机上次控制电流
//	int16_t Last_Control_Current;
//	// 电机停止开启
//	u8 Stop_On;
	// 电机运动标志
	u8 Start_Flag;
	// 电流限幅
	int16_t Current_Limit;
	// 电机路程:由角度乘以系数获得
//	float Round;
//	float Motor_Length;
	// 电机上一次角度
//	int32_t Last_Angle;
	// PID
	struct PID_T PID;
//	FILTER1_HANDLE hfilter1;//惯性滤波器
};
extern struct MOTOR_T Motor[1];

void CAN_Send_Control_Value(void);//发送控制消息
void Data_Convert(u8 Motor_ID); //数据转化
void Motor_Speed_Control(s32 Goal_Speed, u8 Motor_ID); //PID控制
void Motor_Position_Control(s32 Goal_Position, u8 Motor_ID);
void Control_Initialize(void); //电机结构体初始化

//void Motor_Length_Calc(void);  //电机路程计算
//extern u8 Motor_Position_Control(float Goal_Position, int16_t Speed);  //电机位置控制
//extern void Motor_Length_Calc_2(void); //
//void motor_set(u8 id_uart_receive,int pwm_uart_receive);//设定电机的移动
#endif


