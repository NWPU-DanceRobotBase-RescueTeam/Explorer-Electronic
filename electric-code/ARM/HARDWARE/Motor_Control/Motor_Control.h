/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
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
	// ���Ŀ���ٶ�
	s16 Goal_Speed;
};
//typedef struct
//{
//	double a;
//	double last_output;
//}FILTER1_HANDLE, *P_FILTER1_HANDLE;
struct MOTOR_T
{
	// �����۽Ƕ�
	float Current_angle;
	// ��е�Ƕ�
	int32_t Angle;
	// ����ٶ�
	int32_t Speed;
	// �������
	int32_t Moment;
	// ԭʼ�ٶ�
	u8 Original_Data[8];
	// ������Ƶ���
	int16_t Control_Current;
//	//����ϴο��Ƶ���
//	int16_t Last_Control_Current;
//	// ���ֹͣ����
//	u8 Stop_On;
	// ����˶���־
	u8 Start_Flag;
	// �����޷�
	int16_t Current_Limit;
	// ���·��:�ɽǶȳ���ϵ�����
//	float Round;
//	float Motor_Length;
	// �����һ�νǶ�
//	int32_t Last_Angle;
	// PID
	struct PID_T PID;
//	FILTER1_HANDLE hfilter1;//�����˲���
};
extern struct MOTOR_T Motor[1];

void CAN_Send_Control_Value(void);//���Ϳ�����Ϣ
void Data_Convert(u8 Motor_ID); //����ת��
void Motor_Speed_Control(s32 Goal_Speed, u8 Motor_ID); //PID����
void Motor_Position_Control(s32 Goal_Position, u8 Motor_ID);
void Control_Initialize(void); //����ṹ���ʼ��

//void Motor_Length_Calc(void);  //���·�̼���
//extern u8 Motor_Position_Control(float Goal_Position, int16_t Speed);  //���λ�ÿ���
//extern void Motor_Length_Calc_2(void); //
//void motor_set(u8 id_uart_receive,int pwm_uart_receive);//�趨������ƶ�
#endif


