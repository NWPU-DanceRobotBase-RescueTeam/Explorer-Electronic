#include <math.h>
#include "Motor_Control.h"
#include "can.h"

struct M3508_T M3508[4];
int Error_Sum_set;

/**
  * @brief  M3508�ṹ���ʼ��
  * @param  void
  * @retval void
  */
  
void Control_Initialize(void)//ff fb 0a 3a fc 03 20 0e f6 3a 98 3a 98 fe 0d 0a
{
	u8 i;
	for (i = 0; i < 4; i++)
	{
		// �����е��
		M3508[i].Angle = 0;
		// ���Ť��
		M3508[i].Moment = 0;
		// �����ǰ�ٶ�
		M3508[i].Speed = 0;
		// ������������
		M3508[i].Control_Current = 0;
//		M3508[i].Last_Control_Current=0;
//		M3508[i].Stop_On = 0;
//		// ���ֹͣ��־
//		M3508[i].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[i].Current_Limit = 15000;
//		M3508[i].Round = 0;
		M3508[i].Motor_Length = 0;
//		M3508[i].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[i].PID.Kp = 15.1;
		M3508[i].PID.Ki = 0.78;
		M3508[i].PID.Kd = 3.83;
		M3508[i].PID.Last_Error = 0;
		M3508[i].PID.Goal_Speed = 0;
		M3508[i].PID.Error_Sum = 0;
	}
	Error_Sum_set=15000;
}

/**
  * @brief  ��M3508�ش�����ת��Ϊ����ֵ
  * @param  ������(1, 2, 3, 4, 5, 6)
  * @retval void
  */

void Data_Convert(u8 Motor_ID)
{
	M3508[Motor_ID].Angle = (int16_t)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (int16_t)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (int16_t)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/**
  * @brief  ͨ��CAN2���Ϳ����������
  * @param  ������(1, 2, 3, 4, 5, 6)
  * @retval void
  */

void CAN_Send_Control_Value(void)
{
//	u8 i;
	static u8 Motor14_Current[8];
//	static u8 First_Flag = 1;
//	if(First_Flag){
//		for(i = 0; i < 8; ++i)
//		{
//			Motor14_Current[i] = 0;
//		}
//		First_Flag = 0;
//	}
	// ����CAN��
	Motor14_Current[0] = (u8)(((M3508[0].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[1] = (u8)((M3508[0].Control_Current) & 0x00FF);
	
	Motor14_Current[2] = (u8)(((M3508[1].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[3] = (u8)((M3508[1].Control_Current) & 0x00FF);
	
	Motor14_Current[4] = (u8)(((M3508[2].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[5] = (u8)((M3508[2].Control_Current) & 0x00FF);
	
	Motor14_Current[6] = (u8)(((M3508[3].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[7] = (u8)((M3508[3].Control_Current) & 0x00FF);
	
	// ��� 1 ~ 4 0x200
	CAN2_Send_Msg(Motor14_Current, 8, 0x200);
	  //printf("Can Send Msg1 Successfully!");
//	// ��� 5 ~ 8 0x1FF
//	CAN2_Send_Msg(Motor14_Current, 8, 0x1ff);
//	  //printf("Can Send Msg1 Successfully!");
}

/**
  * @brief  ����ٶȿ��ƺ���(PID)(�ڲ��������޷�)  λ��ʽ
  * @param  ������(1, 2, 3, 4, 5, 6)
  *         Goal_Speed:Ŀ���ٶ�
  * @retval void
  */

void Motor_Speed_Control(s16 Goal_Speed, u8 Motor_ID)
{
	double Error;
	double Control_Current = 0;
	//	u8 ki = 0;          //��������ϵ����������ϴ�ʱ���������
		// ����ƫ��
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	if (Error > 200)                                         //Error�˲���ʹ����ƽ��
	{
		Error = 200 * log(Error) - 860;
	}
	else if (Error < -200)
	{
		Error = -(200 * log(-Error) - 860);
	}


	M3508[Motor_ID].PID.Error_Sum += Error;        //���������

	// PID�����޷�
	if (fabs(M3508[Motor_ID].PID.Error_Sum) > Error_Sum_set)
	{
		if (M3508[Motor_ID].PID.Error_Sum > 0) 
			M3508[Motor_ID].PID.Error_Sum = Error_Sum_set;
		else 
			M3508[Motor_ID].PID.Error_Sum = -1 * Error_Sum_set;
	}

//	if((Error-M3508[Motor_ID].PID.Last_Error) < 10 && Error > 10 ) ki = 1;   //��΢����Ϊ�㵫����Ȼ�����ʱ���������

	// ������������
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
		M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum +
		M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;

	// �����޷�
	if (fabs(Control_Current) < 1) Control_Current = 0;  //�˲�ʹ����ƽ��
	else {
		// �޷�
		if (fabs(Control_Current) > M3508[Motor_ID].Current_Limit) 
		{
			if (Control_Current > 0) 
				Control_Current = M3508[Motor_ID].Current_Limit;
			else 
				Control_Current = -M3508[Motor_ID].Current_Limit;
		}
		else 
		{
			Control_Current = (s16)(Control_Current);
		}
	}
	M3508[Motor_ID].Control_Current = Control_Current;
}

/**
  * @brief  ����ٶȿ��ƺ���(PID)(�ڲ��������޷�)  ����ʽ
  * @param  ������(1, 2, 3, 4, 5, 6)
  * @retval void
  */

//void IncPIDCalc(s16 Goal_Speed, u8 Motor_ID)
//{
//	//double Error;
//	double Increment_Control_Current = 0;
//	// ����ƫ��
//	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;

//	// ������������
//	Increment_Control_Current = M3508[Motor_ID].PID.Kp *( M3508[Motor_ID].PID.error-M3508[Motor_ID].PID.Next_Error)+
//										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.error + 
//										M3508[Motor_ID].PID.Kd * (M3508[Motor_ID].PID.error- 2*M3508[Motor_ID].PID.Next_Error + M3508[Motor_ID].PID.Last_Error);
//	// ������ֵ
//	M3508[Motor_ID].Control_Current =M3508[Motor_ID].Control_Current+(int16_t)Increment_Control_Current;
//	M3508[Motor_ID].PID.Last_Error = M3508[Motor_ID].PID.Next_Error;

//	// �����˲�����ֹ������ض���
//	if(fabs(M3508[Motor_ID].Control_Current ) < 1) M3508[Motor_ID].Control_Current  = 0;
//	else{
//		// �޷�
//		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit){
//			if(M3508[Motor_ID].Control_Current  > 0) M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
//			else M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
//		}
//	}
//}

///*
//	����: ���·�̼���
//	����: ��
//	����ֵ: ��
//	��ע: �˺�����Ҫ�Ż�(�����ٶȽ����Ż�)
//*/
//void Motor_Length_Calc(void){
//	u8 i;
//	for(i = 0; i < 2; ++i){
//		// ���ͻ��
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// �Ѿ�һȦ��
//			M3508[i].Round += 360;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// �Ѿ�һȦ��
//			M3508[i].Round -= 360;
//		}
//		M3508[i].Motor_Length = M3508[i].Round + M3508[i].Angle * 0.043951;
//	}
//}

//void Motor_Length_Calc_2(void){
//	// �ٶȻ���
//	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
//}

///*
//	����: ���λ�ÿ���
//	����: ���Ŀ��λ��
//         ����ٶ�(����ֵ)
//	����ֵ: (1 Ϊ�Ѿ��ﵽ)(0 Ϊû�дﵽ)
//*/
//u8 Motor_Position_Control(float Goal_Position, int16_t Speed){
//	u8 Return_Flag = 1;
//	if(fabs(M3508[0].Motor_Length - Goal_Position) > 2000){
//		if(M3508[0].Motor_Length < Goal_Position){
//			// �ٶȵ���������Ҫ��
//			Motor_Speed_Control(Speed, 0);
//			Motor_Speed_Control(-Speed, 1);
//		}
//		else{
//			// �ٶȵ���������Ҫ��
//			Motor_Speed_Control(-Speed, 0);
//			Motor_Speed_Control(Speed, 1);
//		}
//		// δ�ﵽĿ��λ��
//		Return_Flag = 0;
//	}
//	else{
//		Motor_Speed_Control(0, 0);
//		Motor_Speed_Control(0, 1);
//		// �ѴﵽĿ��λ��
//		Return_Flag = 1;
//	}
//	CAN_Send_Control_Value();
//	return Return_Flag;
//}

///*
//	����: �������
//	����: ��
//	����ֵ:(1 ��������) (0 ����������)
//*/
//u8 Motor_Protect(void){
//	static u8 Stop_Count = 0;
//	u8 Protect_Flag = 0;
//	// �������(����)
//	if((fabs(M3508[0].Speed) < 100) || (fabs(M3508[1].Speed) < 100)){
//		if((fabs(M3508[0].Control_Current) > 2000) || (fabs(M3508[1].Control_Current) > 2000)){
//			Stop_Count++;
//		}
//	}
//	if(Stop_Count > 10){
//		Protect_Flag = 1;
//		M3508[0].Stop_Flag = 1;
//		M3508[1].Stop_Flag = 1;
//	}
//	return Protect_Flag;
//}

