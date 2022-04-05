/****************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
�����ţ������Ĵ�0(���ID01)�������Ĵ�1(���ID02)����ǰ���Ĵ�2(���ID03)�����
���Ĵ�3(���ID04)����ǰ���Ĵ�4(���ID05)���Һ��Ĵ�5(���ID06)�������������ǰת��
****************************************************************************/

#include "Motor_Control.h"

struct M3508_T M3508[6];
int Error_Sum_set;
/*
	����: ����ṹ���ʼ������
	����: ��
	����ֵ: ��
*/
void Control_Initialize(void){
	// ���1
	{
		// �����е��
		M3508[0].Angle = 0;
		// ���Ť��
		M3508[0].Moment = 0;
		// �����ǰ�ٶ�
		M3508[0].Speed = 0;
		// ������������
		M3508[0].Control_Current = 0;
		M3508[0].Last_Control_Current=0;
		M3508[0].Stop_On = 0;
		// ���ֹͣ��־
		M3508[0].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[0].Current_Limit = 15000;
		M3508[0].Round = 0;
		M3508[0].Motor_Length = 0;
		M3508[0].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[0].PID.Kp = 14.3;     //p2i1
		M3508[0].PID.Ki = 0.83;
		M3508[0].PID.Kd = 0;
		M3508[0].PID.Last_Error = 0;
		M3508[0].PID.Goal_Speed = 0;
		M3508[0].PID.Error_Sum = 0;
	}
	// ���2
	{
		M3508[1].Angle = 0;
		M3508[1].Moment = 0;
		M3508[1].Speed = 0;
		M3508[1].Control_Current = 0;
		M3508[1].Last_Control_Current=0;
		M3508[1].Stop_On = 0;
		M3508[1].Stop_Flag = 0;
		M3508[1].Current_Limit = 15000;
		M3508[1].Round = 0;
		M3508[1].Motor_Length = 0;
		M3508[1].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[1].PID.Kp = 14.3;
		M3508[1].PID.Ki = 0.83;
		M3508[1].PID.Kd = 0;
		M3508[1].PID.Last_Error = 0;
		M3508[1].PID.Goal_Speed = 0;
		M3508[1].PID.Error_Sum = 0;
	}
	// ���3
	{
		// �����е��
		M3508[2].Angle = 0;
		// ���Ť��
		M3508[2].Moment = 0;
		// �����ǰ�ٶ�
		M3508[2].Speed = 0;
		// ������������
		M3508[2].Control_Current = 0;
		M3508[2].Last_Control_Current=0;
		M3508[2].Stop_On = 0;
		// ���ֹͣ��־
		M3508[2].Stop_Flag = 0;
		// ��������޷�ֵ(����ֵ)
		M3508[2].Current_Limit = 15000;
		M3508[2].Round = 0;
		M3508[2].Motor_Length = 0;
		M3508[2].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[2].PID.Kp = 9;
		M3508[2].PID.Ki = 0.7;//0.5
		M3508[2].PID.Kd = 0;
		M3508[2].PID.Last_Error = 0;
		M3508[2].PID.Goal_Speed = 0;
		M3508[2].PID.Error_Sum = 0;
	}
	// ���4
	{
		M3508[3].Angle = 0;
		M3508[3].Moment = 0;
		M3508[3].Speed = 0;
		M3508[3].Control_Current = 0;
		M3508[3].Last_Control_Current=0;
		M3508[3].Stop_On = 0;
		M3508[3].Stop_Flag = 0;
		M3508[3].Current_Limit = 15000;
		M3508[3].Round = 0;
		M3508[3].Motor_Length = 0;
		M3508[3].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[3].PID.Kp = 9;
		M3508[3].PID.Ki = 0.7;//0.5
		M3508[3].PID.Kd = 0;
		M3508[3].PID.Last_Error = 0;
		M3508[3].PID.Goal_Speed = 0;
		M3508[3].PID.Error_Sum = 0;
	}
	// ���5
	{
		M3508[4].Angle = 0;
		M3508[4].Moment = 0;
		M3508[4].Speed = 0;
		M3508[4].Control_Current = 0;
		M3508[4].Last_Control_Current=0;
		M3508[4].Stop_On = 0;
		M3508[4].Stop_Flag = 0;
		M3508[4].Current_Limit = 15000;
		M3508[4].Round = 0;
		M3508[4].Motor_Length = 0;
		M3508[4].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[4].PID.Kp = 9;
		M3508[4].PID.Ki = 0.7;//0.5
		M3508[4].PID.Kd = 0;
		M3508[4].PID.Last_Error = 0;
		M3508[4].PID.Goal_Speed = 0;
		M3508[4].PID.Error_Sum = 0;
	}
	// ���6
	{
		M3508[5].Angle = 0;
		M3508[5].Moment = 0;
		M3508[5].Speed = 0;
		M3508[5].Control_Current = 0;
		M3508[5].Last_Control_Current=0;
		M3508[5].Stop_On = 0;
		M3508[5].Stop_Flag = 0;
		M3508[5].Current_Limit = 15000;
		M3508[5].Round = 0;
		M3508[5].Motor_Length = 0;
		M3508[5].Last_Angle = 0;
		// PID�ṹ���ʼ��
		M3508[5].PID.Kp = 9;  //10 λ��ʽ PID   
		M3508[5].PID.Ki = 0.7; //0.5
		M3508[5].PID.Kd = 0;  //0.5
		M3508[5].PID.Last_Error = 0;
		M3508[5].PID.error=0;
		M3508[5].PID.Next_Error=0;
		M3508[5].PID.Goal_Speed = 0;
		M3508[5].PID.Error_Sum = 0;
		M3508[5].hfilter1.last_output=0;
	}
	Error_Sum_set=15000;//5000
}


/*
	����: ����ת������(��ԭʼ���ݵõ�������)
	����: ������(1, 2, 3, 4,5,6)
	����ֵ: ��
*/
void Data_Convert(u8 Motor_ID){
	M3508[Motor_ID].Angle = (int16_t)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (int16_t)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (int16_t)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/*
	����: �����������͸����(�Ѵ�����)
	����: ��
	����ֵ: ��
*/
void CAN2_Send_Control_Value(void){
	u8 i;
	static u8 Motor14_Current[8],Motor58_Current[8];
	static u8 First_Flag = 1;
	if(First_Flag){
		for(i = 0; i < 8; ++i)
		{
			Motor14_Current[i] = 0;
			Motor58_Current[i] = 0;
		}
		First_Flag = 0;
	}
	// ����CAN��
	Motor14_Current[0] = (u8)(((M3508[0].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[1] = (u8)((M3508[0].Control_Current) & 0x00FF);
	
	Motor14_Current[2] = (u8)(((M3508[1].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[3] = (u8)((M3508[1].Control_Current) & 0x00FF);
	
	Motor14_Current[4] = (u8)(((M3508[2].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[5] = (u8)((M3508[2].Control_Current) & 0x00FF);
	
	Motor14_Current[6] = (u8)(((M3508[3].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[7] = (u8)((M3508[3].Control_Current) & 0x00FF);
	
	Motor58_Current[0] = (u8)(((M3508[4].Control_Current) & 0xFF00) >> 8);
	Motor58_Current[1] = (u8)((M3508[4].Control_Current) & 0x00FF);
	
	Motor58_Current[2] = (u8)(((M3508[5].Control_Current) & 0xFF00) >> 8);
	Motor58_Current[3] = (u8)((M3508[5].Control_Current) & 0x00FF);
	
	Motor58_Current[4] = 0;
	Motor58_Current[5] = 0;
	
	Motor58_Current[6] = 0;
	Motor58_Current[7] = 0;
	
	// ��� 1 ~ 4 0x200
	// ��� 5 ~ 8 0x1FF
	CAN2_Send_Msg(Motor14_Current, 8, 0x200);
	CAN2_Send_Msg(Motor58_Current, 8, 0x1FF);
}


/*
	����: ����ٶȿ��ƺ���(PID)(�ڲ����޷�)(�����ǻ�ȡ��������������)
	����: Ŀ���ٶ�
         ������(5,6)
	����ֵ: ��
*/
void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID){
	double Error;
	double Control_Current = 0;
	// ����ƫ��
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	if(Error>5000)
	{
		Error=5000;
	}
	else if(Error<-5000)
	{
		Error=-5000;
	}

	M3508[Motor_ID].PID.Error_Sum += Error;
	// PID�����޷�
	if(fabs(M3508[Motor_ID].PID.Error_Sum) > Error_Sum_set){
		if(M3508[Motor_ID].PID.Error_Sum > 0) M3508[Motor_ID].PID.Error_Sum = Error_Sum_set;
		else M3508[Motor_ID].PID.Error_Sum = -1*Error_Sum_set;
	}
	// ������������
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum + 
										M3508[Motor_ID].PID.Kd * (Error-M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;
//	if((Control_Current>0&&M3508[Motor_ID].Control_Current<0)||(Control_Current<0&&M3508[Motor_ID].Control_Current>0))
//	{
//		Control_Current=0;
//	}
////	else if(fabs(Control_Current-M3508[Motor_ID].Control_Current)>100)
////	{
//	else if((Control_Current-M3508[Motor_ID].Control_Current)>50)
//	{
//		Control_Current=M3508[Motor_ID].Control_Current+50;
//	}
//	else if((Control_Current-M3508[Motor_ID].Control_Current)<-50)
//	{
//		Control_Current=M3508[Motor_ID].Control_Current-50;
//	}
	// �����޷�
	if(fabs(Control_Current) < 1) Control_Current = 0;
	else{
		// �޷�
		if(fabs(Control_Current) > M3508[Motor_ID].Current_Limit){
			if(Control_Current > 0) Control_Current = M3508[Motor_ID].Current_Limit;
			else Control_Current = -M3508[Motor_ID].Current_Limit;
		}
		else{
			Control_Current = (int16_t)(Control_Current);
		}
	}
	M3508[Motor_ID].Control_Current = Control_Current;
}


/*
	����: ����ٶȿ��ƺ���(PID) ����ʽ
	����: Ŀ���ٶ�
         ������(5,6)
	����ֵ: ��
*/
void IncPIDCalc(int32_t Goal_Speed, u8 Motor_ID)
{
	//double Error;
	double Increment_Control_Current = 0;
	// ����ƫ��
	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;

	//M3508[Motor_ID].PID.Error_Sum += Error;
	// ������������
	Increment_Control_Current = M3508[Motor_ID].PID.Kp *( M3508[Motor_ID].PID.error-M3508[Motor_ID].PID.Next_Error)+
										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.error + 
										M3508[Motor_ID].PID.Kd * (M3508[Motor_ID].PID.error- 2*M3508[Motor_ID].PID.Next_Error + M3508[Motor_ID].PID.Last_Error);
	//M3508[Motor_ID].PID.Last_Error = Error;
	M3508[Motor_ID].Control_Current =M3508[Motor_ID].Control_Current+(int16_t)Increment_Control_Current;
	M3508[Motor_ID].PID.Last_Error = M3508[Motor_ID].PID.Next_Error;
	M3508[Motor_ID].PID.Next_Error = M3508[Motor_ID].PID.error;
//	if((Control_Current>0&&M3508[Motor_ID].Control_Current<0)||(Control_Current<0&&M3508[Motor_ID].Control_Current>0))
//	{
//		Control_Current=0;
//	}
//	else if(fabs(Control_Current-M3508[Motor_ID].Control_Current)>100)
////	{
//	else if((Control_Current-M3508[Motor_ID].Control_Current)>50)
//	{
//		Control_Current=M3508[Motor_ID].Control_Current+50;
//	}
//	else if((Control_Current-M3508[Motor_ID].Control_Current)<-50)
//	{
//		Control_Current=M3508[Motor_ID].Control_Current-50;
//	}
	// �����޷�
	if(fabs(M3508[Motor_ID].Control_Current ) < 1) M3508[Motor_ID].Control_Current  = 0;
	else{
		// �޷�
		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit){
			if(M3508[Motor_ID].Control_Current  > 0) M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
			else M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
		}
	}
}

/*
	����: ���·�̼���
	����: ��
	����ֵ: ��
	��ע: �˺�����Ҫ�Ż�(�����ٶȽ����Ż�)
*/
//void Motor_Length_Calc(void){
//	u8 i;
//	for(i = 0; i < 2; ++i){
//		// ���ͻ��
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -3000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// �Ѿ�һȦ��
//			M3508[i].Round += 360;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 3000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// �Ѿ�һȦ��
//			M3508[i].Round -= 360;
//		}
//		M3508[i].Motor_Length = M3508[i].Round + M3508[i].Angle * 0.043951;
//	}
//}

void Motor_Length_Calc(void){
	u8 i;
	static u8 First_Flag = 1;
	static int angle_init[2] = {0,0};
	if(First_Flag){
		for(i = 0; i < 2; ++i)
		{
			angle_init[i]= M3508[i].Angle;
			M3508[i].Last_Angle=M3508[i].Angle;
		}
		First_Flag = 0;
	}
	for(i = 0; i < 2; ++i){
		// ���ͻ��
		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000){
		//	M3508[i].Last_Angle = M3508[i].Angle;
		// �Ѿ�һȦ��
		 	M3508[i].Round = M3508[i].Motor_Length;
			angle_init[i]=0;
		}
		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000){
		//	M3508[i].Last_Angle = M3508[i].Angle;
			// �Ѿ�һȦ��
			M3508[i].Round = M3508[i].Motor_Length;
			angle_init[i]=8192;
		}
		if(M3508[i].Speed==0) 
		{
			//M3508[i].Motor_Length=0;
			//angle_init=M3508[i].Angle;
		}
		else M3508[i].Motor_Length = (M3508[i].Round +(M3508[i].Angle-angle_init[i])*0.0043945);
		M3508[i].Last_Angle=M3508[i].Angle;
	}
}


void Motor_Length_Calc_2(void){
	// �ٶȻ���
	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
}

/*
	����: ���λ�ÿ���
	����: ���Ŀ��λ��
         ����ٶ�(����ֵ)
	����ֵ: (1 Ϊ�Ѿ��ﵽ)(0 Ϊû�дﵽ)
*/
u8 Motor_Position_Control(float Goal_Position,u8 Motor_ID){
	u8 Return_Flag = 1;
	float Motor_Position_error;
	int	Set_speed,kp=10;
	Motor_Position_error=M3508[Motor_ID].Motor_Length - Goal_Position;
	if(fabs(Motor_Position_error) > 0.5){
		Set_speed=fabs(Motor_Position_error)*kp;
		if(M3508[Motor_ID].Motor_Length < Goal_Position){
			// �ٶȵ���������Ҫ��
			Motor_Speed_Control(Set_speed, Motor_ID);
		}
		else{
			// �ٶȵ���������Ҫ��
			Motor_Speed_Control(-Set_speed , Motor_ID);
		}
		// δ�ﵽĿ��λ��
		Return_Flag = 0;
	}
	else{
		Motor_Speed_Control(0, 0);
		Motor_Speed_Control(0, 1);
		// �ѴﵽĿ��λ��
		Return_Flag = 1;
	}
	//CAN_Send_Control_Value();
	return Return_Flag;
}

/*
	����: �������
	����: ��
	����ֵ:(1 ��������) (0 ����������)
*/
u8 Motor_Protect(void){
	static u8 Stop_Count = 0;
	u8 Protect_Flag = 0;
	// �������(����)
	if((fabs(M3508[0].Speed) < 100) || (fabs(M3508[1].Speed) < 100)){
		if((fabs(M3508[0].Control_Current) > 2000) || (fabs(M3508[1].Control_Current) > 2000)){
			Stop_Count++;
		}
	}
	if(Stop_Count > 10){
		Protect_Flag = 1;
		M3508[0].Stop_Flag = 1;
		M3508[1].Stop_Flag = 1;
	}
	return Protect_Flag;
}


