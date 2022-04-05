/****************************************************************************
 ���ߣ����δ�Ρ�����
 û���ر����ˣ������ر�Ŭ��

 �����ţ������Ĵ�0(���ID01)�������Ĵ�1(���ID02)����ǰ���Ĵ�2(���ID03)�����
 ���Ĵ�3(���ID04)����ǰ���Ĵ�4(���ID05)���Һ��Ĵ�5(���ID06)�������������ǰת��
 ****************************************************************************/
#include "Motor_Control.h"

struct M3508_T M3508[4];
//GPIO_InitTypeDef relay;
//float shut;

/**
 * @brief  M3508�ṹ���ʼ��
 * @param  void
 * @retval void
 */

void Control_Initialize(void)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		// ���Ĵ��Ƕ�
		M3508[i].Current_angle = 0;
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
		// ���ֹͣ��־
		M3508[i].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[i].Current_Limit = 15000;
//		M3508[i].Round = 0;
		M3508[i].Motor_Length = 0;
//		M3508[i].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[i].PID.Kp = 9;
		M3508[i].PID.Ki = 0.7;		//0.5
		M3508[i].PID.Kd = 0;
		M3508[i].PID.Last_Error = 0;
		M3508[i].PID.Goal_Speed = 0;
		M3508[i].PID.Error_Sum = 0;
		M3508[i].PID.Error_Sum_Limit = 15000;  //5000
	}
}

//void relay_init()
//{
//	relay.GPIO_Pin = GPIO_Pin_9;
//	relay.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//	relay.GPIO_OType = GPIO_OType_PP;//�������
//	relay.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	relay.GPIO_PuPd = GPIO_PuPd_UP;//����
//	GPIO_Init(GPIOA, &relay);//��ʼ��GPIO
//}

//void relay_ON()
//{
//	relay.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_Init(GPIOA, &relay);
//}

//void relay_OFF()
//{
//	relay.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_Init(GPIOA, &relay);
//}

/**
 * @brief  ��M3508�ش�����ת��Ϊ����ֵ
 * @param  ������(1, 2, 3, 4, 5, 6)
 * @retval void
 */

void Data_Convert(uint8_t Motor_ID)
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

void CAN2_Send_Control_Value(void)
{
	static uint8_t Motor14_Current[8];
	// ����CAN��
	Motor14_Current[0] = (uint8_t) (((M3508[0].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[1] = (uint8_t) ((M3508[0].Control_Current) & 0x00FF);

	Motor14_Current[2] = (uint8_t) (((M3508[1].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[3] = (uint8_t) ((M3508[1].Control_Current) & 0x00FF);

	Motor14_Current[4] = (uint8_t) (((M3508[2].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[5] = (uint8_t) ((M3508[2].Control_Current) & 0x00FF);

	Motor14_Current[6] = (uint8_t) (((M3508[3].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[7] = (uint8_t) ((M3508[3].Control_Current) & 0x00FF);

	// ��� 1 ~ 4 0x200
	CAN2_Send_Msg(Motor14_Current, 0x200);
}

/**
 * @brief  ����ٶȿ��ƺ���(PID)(�ڲ��������޷�)  λ��ʽ
 * @param  ������(1, 2, 3, 4, 5, 6)
 *         Goal_Speed:Ŀ���ٶ�
 * @retval void
 */

void Motor_Speed_Control(int16_t Goal_Speed, uint8_t Motor_ID)
{
	double Error;
	double Control_Current = 0;
	// ����ƫ��
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	if (Error > 5000)
	{
		Error = 5000;
	}
	else if (Error < -5000)
	{
		Error = -5000;
	}

	M3508[Motor_ID].PID.Error_Sum += Error;
	// PID�����޷�
	if (fabs(M3508[Motor_ID].PID.Error_Sum) > M3508[Motor_ID].PID.Error_Sum_Limit)
	{
		if (M3508[Motor_ID].PID.Error_Sum > 0)
			M3508[Motor_ID].PID.Error_Sum = M3508[Motor_ID].PID.Error_Sum_Limit;
		else
			M3508[Motor_ID].PID.Error_Sum = -1 * M3508[Motor_ID].PID.Error_Sum_Limit;
	}
	// ������������
	Control_Current = M3508[Motor_ID].PID.Kp * Error + M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum + M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;
	// �����޷�
	if (fabs(Control_Current) < 1)
		Control_Current = 0;
	else
	{
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
			Control_Current = (int16_t) (Control_Current);
		}
	}
	M3508[Motor_ID].Control_Current = Control_Current;
}

/**
 * @brief  ����ٶȿ��ƺ���(PID)(�ڲ��������޷�)  ����ʽ
 * @param  ������(1, 2, 3, 4, 5, 6)
 * @retval void
 */

//void IncPIDCalc(int32_t Goal_Speed, u8 Motor_ID)
//{
//	//double Error;
//	double Increment_Control_Current = 0;
//	// ����ƫ��
//	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;
//	//M3508[Motor_ID].PID.Error_Sum += Error;
//	// ������������
//	Increment_Control_Current = M3508[Motor_ID].PID.Kp *( M3508[Motor_ID].PID.error-M3508[Motor_ID].PID.Next_Error)
//							  + M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.error 
//						      + M3508[Motor_ID].PID.Kd * (M3508[Motor_ID].PID.error- 2*M3508[Motor_ID].PID.Next_Error 
//	                          + M3508[Motor_ID].PID.Last_Error);
//	//M3508[Motor_ID].PID.Last_Error = Error;
//	M3508[Motor_ID].Control_Current = M3508[Motor_ID].Control_Current+(int16_t)Increment_Control_Current;
//	M3508[Motor_ID].PID.Last_Error = M3508[Motor_ID].PID.Next_Error;
//	M3508[Motor_ID].PID.Next_Error = M3508[Motor_ID].PID.error;
////	if((Control_Current>0&&M3508[Motor_ID].Control_Current<0)||(Control_Current<0&&M3508[Motor_ID].Control_Current>0))
////	{
////		Control_Current=0;
////	}
////	else if(fabs(Control_Current-M3508[Motor_ID].Control_Current)>100)
//////	{
////	else if((Control_Current-M3508[Motor_ID].Control_Current)>50)
////	{
////		Control_Current=M3508[Motor_ID].Control_Current+50;
////	}
////	else if((Control_Current-M3508[Motor_ID].Control_Current)<-50)
////	{
////		Control_Current=M3508[Motor_ID].Control_Current-50;
////	}
//	// �����޷�
//	if(fabs(M3508[Motor_ID].Control_Current ) < 1) 
//		M3508[Motor_ID].Control_Current  = 0;
//	else
//	{
//		// �޷�
//		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit)
//		{
//			if(M3508[Motor_ID].Control_Current  > 0) 
//				M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
//			else 
//				M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
//		}
//	}
//}
/**
 * @brief  ���·�̼���
 * @param  ������(1, 2, 3, 4, 5, 6)
 * @retval void
 * @note   �˺�����Ҫ�Ż�(�����ٶȽ����Ż�)
 */

//void Motor_Length_Calc(void){
//	u8 i;
//	static u8 First_Flag = 1;
//	static int angle_init[2] = {0,0};
//	if(First_Flag)
//	{
//		for(i = 0; i < 2; ++i)
//		{
//			angle_init[i]= M3508[i].Angle;
//			M3508[i].Last_Angle=M3508[i].Angle;
//		}
//		First_Flag = 0;
//	}
//	for(i = 0; i < 2; ++i){
//		// ���ͻ��
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//		// �Ѿ�һȦ��
//		 	M3508[i].Round = M3508[i].Motor_Length;
//			angle_init[i]=0;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//			// �Ѿ�һȦ��
//			M3508[i].Round = M3508[i].Motor_Length;
//			angle_init[i]=8192;
//		}
//		if(M3508[i].Speed==0) 
//		{
//			//M3508[i].Motor_Length=0;
//			//angle_init=M3508[i].Angle;
//		}
//		else M3508[i].Motor_Length = (M3508[i].Round +(M3508[i].Angle-angle_init[i])*0.0043945f);
//		M3508[i].Last_Angle=M3508[i].Angle;
//	}
//}
//void Motor_Length_Calc_2(void){
//	// �ٶȻ���
//	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
//}
/**
 * @brief  ���λ�ÿ���
 * @param  Goal_Position:Ŀ��λ��
 *         Motor_ID:������(1, 2, 3, 4, 5, 6)
 * @retval 1:�ﵽĿ��λ��,0:δ�ﵽĿ��λ��
 */

//u8 Motor_Position_Control(float Goal_Position,u8 Motor_ID){
//	u8 Return_Flag = 1;
//	float Motor_Position_error;
//	int	Set_speed,kp=10;
//	Motor_Position_error=M3508[Motor_ID].Motor_Length - Goal_Position;
//	Set_speed=fabs(Motor_Position_error)*kp;
//	if(fabs(Motor_Position_error) > 0.5)
//	{
//		if(M3508[Motor_ID].Motor_Length < Goal_Position)
//		{
//			// �ٶȵ���������Ҫ��
//			Motor_Speed_Control(Set_speed, Motor_ID);
//		}
//		else{
//			// �ٶȵ���������Ҫ��
//			Motor_Speed_Control(-Set_speed , Motor_ID);
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
//	return Return_Flag;
//}
/**
 * @brief  �������
 * @param  void
 * @retval 1:��������,0:δ��������
 */

//uint8_t Motor_Protect(void) {
//	static uint8_t Stop_Count = 0;
//	uint8_t Protect_Flag = 0;
//	// �������(����)
//	if ((fabs(M3508[0].Speed) < 100) || (fabs(M3508[1].Speed) < 100))
//	{
//		if ((fabs(M3508[0].Control_Current) > 2000) || (fabs(M3508[1].Control_Current) > 2000))
//		{
//			Stop_Count++;
//		}
//	}
//	if (Stop_Count > 10)
//	{
//		Protect_Flag = 1;
//		M3508[0].Start_Flag = 0;
//		M3508[1].Start_Flag = 1;
//	}
//	return Protect_Flag;
//}

