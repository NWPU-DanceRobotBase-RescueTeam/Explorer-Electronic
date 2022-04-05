/****************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
电机编号：左主履带0(电调ID01)，右主履带1(电调ID02)，左前副履带2(电调ID03)，左后
副履带3(电调ID04)，右前副履带4(电调ID05)，右后副履带5(电调ID06)（左电机反向电流前转）
****************************************************************************/

#include "Motor_Control.h"

struct M3508_T M3508[6];
int Error_Sum_set;
/*
	描述: 电机结构体初始化函数
	参数: 无
	返回值: 无
*/
void Control_Initialize(void){
	// 电调1
	{
		// 电机机械角
		M3508[0].Angle = 0;
		// 电机扭矩
		M3508[0].Moment = 0;
		// 电机当前速度
		M3508[0].Speed = 0;
		// 电机电流输出量
		M3508[0].Control_Current = 0;
		M3508[0].Last_Control_Current=0;
		M3508[0].Stop_On = 0;
		// 电机停止标志
		M3508[0].Stop_Flag = 0;
		// 电机电流限幅值(绝对值)
		M3508[0].Current_Limit = 15000;
		M3508[0].Round = 0;
		M3508[0].Motor_Length = 0;
		M3508[0].Last_Angle = 0;
		// PID结构体初始化
		M3508[0].PID.Kp = 14.3;     //p2i1
		M3508[0].PID.Ki = 0.83;
		M3508[0].PID.Kd = 0;
		M3508[0].PID.Last_Error = 0;
		M3508[0].PID.Goal_Speed = 0;
		M3508[0].PID.Error_Sum = 0;
	}
	// 电调2
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
		// PID结构体初始化
		M3508[1].PID.Kp = 14.3;
		M3508[1].PID.Ki = 0.83;
		M3508[1].PID.Kd = 0;
		M3508[1].PID.Last_Error = 0;
		M3508[1].PID.Goal_Speed = 0;
		M3508[1].PID.Error_Sum = 0;
	}
	// 电调3
	{
		// 电机机械角
		M3508[2].Angle = 0;
		// 电机扭矩
		M3508[2].Moment = 0;
		// 电机当前速度
		M3508[2].Speed = 0;
		// 电机电流输出量
		M3508[2].Control_Current = 0;
		M3508[2].Last_Control_Current=0;
		M3508[2].Stop_On = 0;
		// 电机停止标志
		M3508[2].Stop_Flag = 0;
		// 电机电流限幅值(绝对值)
		M3508[2].Current_Limit = 15000;
		M3508[2].Round = 0;
		M3508[2].Motor_Length = 0;
		M3508[2].Last_Angle = 0;
		// PID结构体初始化
		M3508[2].PID.Kp = 9;
		M3508[2].PID.Ki = 0.7;//0.5
		M3508[2].PID.Kd = 0;
		M3508[2].PID.Last_Error = 0;
		M3508[2].PID.Goal_Speed = 0;
		M3508[2].PID.Error_Sum = 0;
	}
	// 电调4
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
		// PID结构体初始化
		M3508[3].PID.Kp = 9;
		M3508[3].PID.Ki = 0.7;//0.5
		M3508[3].PID.Kd = 0;
		M3508[3].PID.Last_Error = 0;
		M3508[3].PID.Goal_Speed = 0;
		M3508[3].PID.Error_Sum = 0;
	}
	// 电调5
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
		// PID结构体初始化
		M3508[4].PID.Kp = 9;
		M3508[4].PID.Ki = 0.7;//0.5
		M3508[4].PID.Kd = 0;
		M3508[4].PID.Last_Error = 0;
		M3508[4].PID.Goal_Speed = 0;
		M3508[4].PID.Error_Sum = 0;
	}
	// 电调6
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
		// PID结构体初始化
		M3508[5].PID.Kp = 9;  //10 位置式 PID   
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
	描述: 数据转换函数(由原始数据得到物理量)
	参数: 电机电调(1, 2, 3, 4,5,6)
	返回值: 无
*/
void Data_Convert(u8 Motor_ID){
	M3508[Motor_ID].Angle = (int16_t)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (int16_t)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (int16_t)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/*
	描述: 将控制量发送给电机(已带保护)
	参数: 无
	返回值: 无
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
	// 填入CAN包
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
	
	// 电调 1 ~ 4 0x200
	// 电调 5 ~ 8 0x1FF
	CAN2_Send_Msg(Motor14_Current, 8, 0x200);
	CAN2_Send_Msg(Motor58_Current, 8, 0x1FF);
}


/*
	描述: 电机速度控制函数(PID)(内部带限幅)(仅仅是获取控制量，不发送)
	参数: 目标速度
         电机电调(5,6)
	返回值: 无
*/
void Motor_Speed_Control(int32_t Goal_Speed, u8 Motor_ID){
	double Error;
	double Control_Current = 0;
	// 计算偏差
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
	// PID积分限幅
	if(fabs(M3508[Motor_ID].PID.Error_Sum) > Error_Sum_set){
		if(M3508[Motor_ID].PID.Error_Sum > 0) M3508[Motor_ID].PID.Error_Sum = Error_Sum_set;
		else M3508[Motor_ID].PID.Error_Sum = -1*Error_Sum_set;
	}
	// 计算电流输出量
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
	// 电流限幅
	if(fabs(Control_Current) < 1) Control_Current = 0;
	else{
		// 限幅
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
	描述: 电机速度控制函数(PID) 增量式
	参数: 目标速度
         电机电调(5,6)
	返回值: 无
*/
void IncPIDCalc(int32_t Goal_Speed, u8 Motor_ID)
{
	//double Error;
	double Increment_Control_Current = 0;
	// 计算偏差
	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;

	//M3508[Motor_ID].PID.Error_Sum += Error;
	// 计算电流输出量
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
	// 电流限幅
	if(fabs(M3508[Motor_ID].Control_Current ) < 1) M3508[Motor_ID].Control_Current  = 0;
	else{
		// 限幅
		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit){
			if(M3508[Motor_ID].Control_Current  > 0) M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
			else M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
		}
	}
}

/*
	描述: 电机路程计算
	参数: 无
	返回值: 无
	备注: 此函数需要优化(根据速度进行优化)
*/
//void Motor_Length_Calc(void){
//	u8 i;
//	for(i = 0; i < 2; ++i){
//		// 检测突变
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -3000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
//			M3508[i].Round += 360;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 3000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
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
		// 检测突变
		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000){
		//	M3508[i].Last_Angle = M3508[i].Angle;
		// 已经一圈了
		 	M3508[i].Round = M3508[i].Motor_Length;
			angle_init[i]=0;
		}
		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000){
		//	M3508[i].Last_Angle = M3508[i].Angle;
			// 已经一圈了
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
	// 速度积分
	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
}

/*
	描述: 电机位置控制
	参数: 电机目标位置
         电机速度(绝对值)
	返回值: (1 为已经达到)(0 为没有达到)
*/
u8 Motor_Position_Control(float Goal_Position,u8 Motor_ID){
	u8 Return_Flag = 1;
	float Motor_Position_error;
	int	Set_speed,kp=10;
	Motor_Position_error=M3508[Motor_ID].Motor_Length - Goal_Position;
	if(fabs(Motor_Position_error) > 0.5){
		Set_speed=fabs(Motor_Position_error)*kp;
		if(M3508[Motor_ID].Motor_Length < Goal_Position){
			// 速度的正负号需要调
			Motor_Speed_Control(Set_speed, Motor_ID);
		}
		else{
			// 速度的正负号需要调
			Motor_Speed_Control(-Set_speed , Motor_ID);
		}
		// 未达到目标位置
		Return_Flag = 0;
	}
	else{
		Motor_Speed_Control(0, 0);
		Motor_Speed_Control(0, 1);
		// 已达到目标位置
		Return_Flag = 1;
	}
	//CAN_Send_Control_Value();
	return Return_Flag;
}

/*
	描述: 电机保护
	参数: 无
	返回值:(1 启动保护) (0 不启动保护)
*/
u8 Motor_Protect(void){
	static u8 Stop_Count = 0;
	u8 Protect_Flag = 0;
	// 保护检测(条件)
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


