/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __MOTOR_CONTROL__
#define __MOTOR_CONTROL__

#include "main.h"

struct PID_T
{
	float Kp;
	float Ki;
	float Kd;
	float Last_Error;
	float error;
	float Next_Error;
	float Error_Sum;
	// ����޷�
	int16_t Error_Sum_Limit;
	// ���Ŀ���ٶ�
	int16_t Goal_Speed;
	// ���Ŀ��λ��
	int16_t Goal_Position;
};
//typedef struct
//{
//	double a;
//	double last_output;
//}FILTER1_HANDLE, *P_FILTER1_HANDLE;

struct M3508_T
{
	// ���Ĵ��Ƕ�
	float Current_angle;
	// ����Ƕ�
	int16_t Angle;
	// ����ٶ�
	int16_t Speed;
	// �������
	int16_t Moment;
	// ԭʼ�ٶ�
	uint8_t Original_Data[8];
	// ������Ƶ���
	int16_t Control_Current;
	//����ϴο��Ƶ���
//	int16_t Last_Control_Current;
	// ���ֹͣ����
//	u8 Stop_On;
	// ���ֹͣ��־
	uint8_t Stop_Flag;
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

extern struct M3508_T M3508[4];  // �ĸ�M3508���Ĵ����

//extern float shut;
//extern GPIO_InitTypeDef relay;

void CAN2_Send_Control_Value(void);   //���Ϳ�����Ϣ
void Data_Convert(uint8_t Motor_ID);       //����ת��
void Motor_Speed_Control(int16_t Goal_Speed, uint8_t Motor_ID); //PID����
void Control_Initialize(void);       //����ṹ���ʼ��
//uint8_t Motor_Protect(void);              //�������
//extern void relay_init(void);
//extern void relay_ON(void);
//extern void relay_OFF(void);

#endif


