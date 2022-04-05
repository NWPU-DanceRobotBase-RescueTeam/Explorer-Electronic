/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__
#include "sys.h"
#include <math.h>
#include "can.h"


struct PID_T{
	double Kp;
	double Ki;
	double Kd;
	double Last_Error;
	double error;
	double Next_Error;
	double Error_Sum;
	// ���Ŀ���ٶ�
	int32_t Goal_Speed;
	// ���Ŀ��λ��
	int16_t  Goal_Position;
};
typedef struct
{
	double a;
	double last_output;
}FILTER1_HANDLE, *P_FILTER1_HANDLE;
struct M3508_T{
	// ��λ���Ƕ�
	float Angle;
	// ����ٶ�
	int32_t Speed;
	// �������
	int32_t Moment;
	// ԭʼ�ٶ�
	u8 Original_Data[8];
	// ������Ƶ���
	int16_t Control_Current;
	//����ϴο��Ƶ���
	int16_t Last_Control_Current;
	// ���ֹͣ����
	u8 Stop_On;
	// ���ֹͣ��־
	u8 Stop_Flag;
	// �����޷�
	int16_t Current_Limit;
	// ���·��:�ɽǶȳ���ϵ�����
	float Round;
	float Motor_Length;
	// �����һ�νǶ�
	int32_t Last_Angle;
	// PID
	struct PID_T PID;
	FILTER1_HANDLE hfilter1;//�����˲���
};
extern struct M3508_T M3508[6];
extern int Error_Sum_set;

extern void CAN2_Send_Control_Value(void);//���Ϳ�����Ϣ
extern void Data_Convert(u8 Motor_ID); //����ת��
extern void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID); //PID����
extern void Control_Initialize(void); //����ṹ���ʼ��
extern void Motor_Length_Calc(void);  //���·�̼���
extern u8 Motor_Position_Control(float Goal_Position, u8 Motor_ID);  //���λ�ÿ���
extern void Motor_Length_Calc_2(void); //
extern u8 Motor_Protect(void);  //�������
#endif


