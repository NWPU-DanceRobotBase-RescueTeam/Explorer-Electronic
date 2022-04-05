/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include "sys.h"

struct PID_T
{
	float Kp;
	float Ki;
	float Kd;
	float Last_Error;
	float error;
	float Next_Error;
	float Error_Sum;
	// 误差限幅
	s16 Error_Sum_Limit;
	// 电机目标速度
	s16 Goal_Speed;
	// 电机目标位置
	s16 Goal_Position;
};
//typedef struct
//{
//	double a;
//	double last_output;
//}FILTER1_HANDLE, *P_FILTER1_HANDLE;

struct M3508_T
{
	// 副履带角度
	float Current_angle;
	// 电机角度
	float Angle;
	// 电机速度
	int32_t Speed;
	// 电机力矩
	int32_t Moment;
	// 原始速度
	u8 Original_Data[8];
	// 电机控制电流
	int16_t Control_Current;
	//电机上次控制电流
//	int16_t Last_Control_Current;
	// 电机停止开启
//	u8 Stop_On;
	// 电机停止标志
	u8 Start_Flag;
	// 电流限幅
	int16_t Current_Limit;
	// 电机路程:由角度乘以系数获得
//	float Round;
	float Motor_Length;
	// 电机上一次角度
//	int32_t Last_Angle;
	// PID
	struct PID_T PID;
//	FILTER1_HANDLE hfilter1;//惯性滤波器
};

extern struct M3508_T M3508[4];  // 四个M3508副履带电机

//extern float shut;
//extern GPIO_InitTypeDef relay;

void CAN2_Send_Control_Value(void);   //发送控制消息
void Data_Convert(u8 Motor_ID);       //数据转化
void Motor_Speed_Control(s16 Goal_Speed, u8 Motor_ID); //PID控制
void Control_Initialize(void);       //电机结构体初始化
//void Motor_Length_Calc(void);       //电机路程计算
//u8 Motor_Position_Control(float Goal_Position, u8 Motor_ID);  //电机位置控制
//void Motor_Length_Calc_2(void); 
u8 Motor_Protect(void);              //电机保护
//extern void relay_init(void);
//extern void relay_ON(void);
//extern void relay_OFF(void);

#endif


