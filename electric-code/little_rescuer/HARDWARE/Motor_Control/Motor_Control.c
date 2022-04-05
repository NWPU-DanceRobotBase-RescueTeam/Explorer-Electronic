#include <math.h>
#include "Motor_Control.h"
#include "can.h"

struct M3508_T M3508[4];
int Error_Sum_set;

/**
  * @brief  M3508结构体初始化
  * @param  void
  * @retval void
  */
  
void Control_Initialize(void)//ff fb 0a 3a fc 03 20 0e f6 3a 98 3a 98 fe 0d 0a
{
	u8 i;
	for (i = 0; i < 4; i++)
	{
		// 电机机械角
		M3508[i].Angle = 0;
		// 电机扭矩
		M3508[i].Moment = 0;
		// 电机当前速度
		M3508[i].Speed = 0;
		// 电机电流输出量
		M3508[i].Control_Current = 0;
//		M3508[i].Last_Control_Current=0;
//		M3508[i].Stop_On = 0;
//		// 电机停止标志
//		M3508[i].Stop_Flag = 0;
		// 电机电流限幅值(绝对值)
		M3508[i].Current_Limit = 15000;
//		M3508[i].Round = 0;
		M3508[i].Motor_Length = 0;
//		M3508[i].Last_Angle = 0;
		// PID结构体初始化
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
  * @brief  将M3508回传数据转换为物理值
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  */

void Data_Convert(u8 Motor_ID)
{
	M3508[Motor_ID].Angle = (int16_t)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (int16_t)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (int16_t)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/**
  * @brief  通过CAN2发送控制量给电机
  * @param  电机电调(1, 2, 3, 4, 5, 6)
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
	// 填入CAN包
	Motor14_Current[0] = (u8)(((M3508[0].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[1] = (u8)((M3508[0].Control_Current) & 0x00FF);
	
	Motor14_Current[2] = (u8)(((M3508[1].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[3] = (u8)((M3508[1].Control_Current) & 0x00FF);
	
	Motor14_Current[4] = (u8)(((M3508[2].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[5] = (u8)((M3508[2].Control_Current) & 0x00FF);
	
	Motor14_Current[6] = (u8)(((M3508[3].Control_Current) & 0xFF00) >> 8);
	Motor14_Current[7] = (u8)((M3508[3].Control_Current) & 0x00FF);
	
	// 电调 1 ~ 4 0x200
	CAN2_Send_Msg(Motor14_Current, 8, 0x200);
	  //printf("Can Send Msg1 Successfully!");
//	// 电调 5 ~ 8 0x1FF
//	CAN2_Send_Msg(Motor14_Current, 8, 0x1ff);
//	  //printf("Can Send Msg1 Successfully!");
}

/**
  * @brief  电机速度控制函数(PID)(内部带积分限幅)  位置式
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  *         Goal_Speed:目标速度
  * @retval void
  */

void Motor_Speed_Control(s16 Goal_Speed, u8 Motor_ID)
{
	double Error;
	double Control_Current = 0;
	//	u8 ki = 0;          //积分量的系数，当定差较大时加入积分量
		// 计算偏差
	Error = Goal_Speed - M3508[Motor_ID].Speed;
	if (Error > 200)                                         //Error滤波，使曲线平滑
	{
		Error = 200 * log(Error) - 860;
	}
	else if (Error < -200)
	{
		Error = -(200 * log(-Error) - 860);
	}


	M3508[Motor_ID].PID.Error_Sum += Error;        //计算积分量

	// PID积分限幅
	if (fabs(M3508[Motor_ID].PID.Error_Sum) > Error_Sum_set)
	{
		if (M3508[Motor_ID].PID.Error_Sum > 0) 
			M3508[Motor_ID].PID.Error_Sum = Error_Sum_set;
		else 
			M3508[Motor_ID].PID.Error_Sum = -1 * Error_Sum_set;
	}

//	if((Error-M3508[Motor_ID].PID.Last_Error) < 10 && Error > 10 ) ki = 1;   //当微分量为零但是依然有误差时加入积分量

	// 计算电流输出量
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
		M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum +
		M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
	M3508[Motor_ID].PID.Last_Error = Error;

	// 电流限幅
	if (fabs(Control_Current) < 1) Control_Current = 0;  //滤波使曲线平滑
	else {
		// 限幅
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
  * @brief  电机速度控制函数(PID)(内部带积分限幅)  增量式
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  */

//void IncPIDCalc(s16 Goal_Speed, u8 Motor_ID)
//{
//	//double Error;
//	double Increment_Control_Current = 0;
//	// 计算偏差
//	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;

//	// 计算电流输出量
//	Increment_Control_Current = M3508[Motor_ID].PID.Kp *( M3508[Motor_ID].PID.error-M3508[Motor_ID].PID.Next_Error)+
//										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.error + 
//										M3508[Motor_ID].PID.Kd * (M3508[Motor_ID].PID.error- 2*M3508[Motor_ID].PID.Next_Error + M3508[Motor_ID].PID.Last_Error);
//	// 更新数值
//	M3508[Motor_ID].Control_Current =M3508[Motor_ID].Control_Current+(int16_t)Increment_Control_Current;
//	M3508[Motor_ID].PID.Last_Error = M3508[Motor_ID].PID.Next_Error;

//	// 电流滤波，防止电机来回抖动
//	if(fabs(M3508[Motor_ID].Control_Current ) < 1) M3508[Motor_ID].Control_Current  = 0;
//	else{
//		// 限幅
//		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit){
//			if(M3508[Motor_ID].Control_Current  > 0) M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
//			else M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
//		}
//	}
//}

///*
//	描述: 电机路程计算
//	参数: 无
//	返回值: 无
//	备注: 此函数需要优化(根据速度进行优化)
//*/
//void Motor_Length_Calc(void){
//	u8 i;
//	for(i = 0; i < 2; ++i){
//		// 检测突变
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
//			M3508[i].Round += 360;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000){
//			M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
//			M3508[i].Round -= 360;
//		}
//		M3508[i].Motor_Length = M3508[i].Round + M3508[i].Angle * 0.043951;
//	}
//}

//void Motor_Length_Calc_2(void){
//	// 速度积分
//	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
//}

///*
//	描述: 电机位置控制
//	参数: 电机目标位置
//         电机速度(绝对值)
//	返回值: (1 为已经达到)(0 为没有达到)
//*/
//u8 Motor_Position_Control(float Goal_Position, int16_t Speed){
//	u8 Return_Flag = 1;
//	if(fabs(M3508[0].Motor_Length - Goal_Position) > 2000){
//		if(M3508[0].Motor_Length < Goal_Position){
//			// 速度的正负号需要调
//			Motor_Speed_Control(Speed, 0);
//			Motor_Speed_Control(-Speed, 1);
//		}
//		else{
//			// 速度的正负号需要调
//			Motor_Speed_Control(-Speed, 0);
//			Motor_Speed_Control(Speed, 1);
//		}
//		// 未达到目标位置
//		Return_Flag = 0;
//	}
//	else{
//		Motor_Speed_Control(0, 0);
//		Motor_Speed_Control(0, 1);
//		// 已达到目标位置
//		Return_Flag = 1;
//	}
//	CAN_Send_Control_Value();
//	return Return_Flag;
//}

///*
//	描述: 电机保护
//	参数: 无
//	返回值:(1 启动保护) (0 不启动保护)
//*/
//u8 Motor_Protect(void){
//	static u8 Stop_Count = 0;
//	u8 Protect_Flag = 0;
//	// 保护检测(条件)
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

