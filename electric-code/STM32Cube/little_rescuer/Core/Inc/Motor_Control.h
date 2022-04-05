/**************************************************************************
 ���ߣ����δ��
 û���ر����ˣ������ر�Ŭ��
 **************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include "main.h"


struct PID_T
{
	double Kp;
	double Ki;
	double Kd;
	double Last_Error;
	double error;
	double Next_Error;
	double Error_Sum;
	// ���Ŀ���ٶ�
	int16_t Goal_Speed;
};
//typedef struct
//{
//	double a;
//	double last_output;
//}FILTER1_HANDLE, *P_FILTER1_HANDLE;
struct M3508_T
{
	// ��е�Ƕ�
	int32_t Angle;
	// ����ٶ�
	int32_t Speed;
	// �������
	int32_t Moment;
	// ԭʼ�ٶ�
	uint8_t Original_Data[8];
	// ������Ƶ���
	int16_t Control_Current;
//	//����ϴο��Ƶ���
//	int16_t Last_Control_Current;
//	// ���ֹͣ����
//	uint8_t Stop_On;
//	// ���ֹͣ��־
//	uint8_t Stop_Flag;
	// �����޷�
	int16_t Current_Limit;
	// ���·��:�ɽǶȳ���ϵ�����
//	float Round;
	float Motor_Length;
	// �����һ�νǶ�
//	int32_t Last_Angle;
	// PID
	struct PID_T PID;
//	FILTER1_HANDLE hfilter1;//�����˲���
};
extern struct M3508_T M3508[4];
extern int Error_Sum_set;

extern void CAN_Send_Control_Value(void);//���Ϳ�����Ϣ
extern void Data_Convert(uint8_t Motor_ID); //����ת��
extern void Motor_Speed_Control(int16_t Goal_Speed, uint8_t Motor_ID); //PID����
extern void Control_Initialize(void); //����ṹ���ʼ��
//extern void Motor_Length_Calc(void);  //���·�̼���
//extern void Motor_Length_Calc_2(void); //
//extern uint8_t Motor_Protect(void);  //�������
#endif

