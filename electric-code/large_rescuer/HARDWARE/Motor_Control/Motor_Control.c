/****************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力

电机编号：左主履带0(电调ID01)，右主履带1(电调ID02)，左前副履带2(电调ID03)，左后
副履带3(电调ID04)，右前副履带4(电调ID05)，右后副履带5(电调ID06)（左电机反向电流前转）
****************************************************************************/
#include <math.h>
#include "Motor_Control.h"
#include "can.h"

struct M3508_T M3508[4];
//GPIO_InitTypeDef relay;
//float shut;

/**
  * @brief  M3508结构体初始化
  * @param  void
  * @retval void
  */

void Control_Initialize(void)
{
	u8 i;
	for (i = 0; i < 4; i++)
	{
		// 副履带角度
		M3508[i].Current_angle = 0;
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
		// 电机停止标志
		M3508[i].Start_Flag = 0;
		// 电机电流限幅值(绝对值)
		M3508[i].Current_Limit = 15000;
//		M3508[i].Round = 0;
		M3508[i].Motor_Length = 0;
//		M3508[i].Last_Angle = 0;
		// PID结构体初始化
		M3508[i].PID.Kp = 9;
		M3508[i].PID.Ki = 0.7;//0.5
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
//	relay.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	relay.GPIO_OType = GPIO_OType_PP;//推挽输出
//	relay.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	relay.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	GPIO_Init(GPIOA, &relay);//初始化GPIO
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
  * @brief  将M3508回传数据转换为物理值
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  */

void Data_Convert(u8 Motor_ID)
{
	M3508[Motor_ID].Angle = (s16)((M3508[Motor_ID].Original_Data[0] << 8) + M3508[Motor_ID].Original_Data[1]);
	M3508[Motor_ID].Speed = (s16)((M3508[Motor_ID].Original_Data[2] << 8) + M3508[Motor_ID].Original_Data[3]);
	M3508[Motor_ID].Moment = (s16)((M3508[Motor_ID].Original_Data[4] << 8) + M3508[Motor_ID].Original_Data[5]);
}

/**
  * @brief  通过CAN2发送控制量给电机
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  */

void CAN2_Send_Control_Value(void) {
	u8 i;
	static u8 Motor14_Current[8];
	static u8 First_Flag = 1;
	if (First_Flag)
	{
		for (i = 0; i < 8; ++i)
		{
			Motor14_Current[i] = 0;
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

	// 电调 1 ~ 4 0x200
	CAN2_Send_Msg(Motor14_Current, 8, 0x200);
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
	// 计算偏差
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
	// PID积分限幅
	if (fabs(M3508[Motor_ID].PID.Error_Sum) > M3508[Motor_ID].PID.Error_Sum_Limit)
	{
		if (M3508[Motor_ID].PID.Error_Sum > 0)
			M3508[Motor_ID].PID.Error_Sum = M3508[Motor_ID].PID.Error_Sum_Limit;
		else
			M3508[Motor_ID].PID.Error_Sum = -1 * M3508[Motor_ID].PID.Error_Sum_Limit;
	}
	// 计算电流输出量
	Control_Current = M3508[Motor_ID].PID.Kp * Error +
		M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.Error_Sum +
		M3508[Motor_ID].PID.Kd * (Error - M3508[Motor_ID].PID.Last_Error);
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
	if (fabs(Control_Current) < 1)
		Control_Current = 0;
	else
	{
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
			Control_Current = (int16_t)(Control_Current);
		}
	}
	M3508[Motor_ID].Control_Current = Control_Current;
}

/**
  * @brief  电机速度控制函数(PID)(内部带积分限幅)  增量式
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  */

//void IncPIDCalc(int32_t Goal_Speed, u8 Motor_ID)
//{
//	//double Error;
//	double Increment_Control_Current = 0;
//	// 计算偏差
//	M3508[Motor_ID].PID.error = Goal_Speed - M3508[Motor_ID].Speed;

//	//M3508[Motor_ID].PID.Error_Sum += Error;
//	// 计算电流输出量
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
//	// 电流限幅
//	if(fabs(M3508[Motor_ID].Control_Current ) < 1) 
//		M3508[Motor_ID].Control_Current  = 0;
//	else
//	{
//		// 限幅
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
  * @brief  电机路程计算
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  * @note   此函数需要优化(根据速度进行优化)
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
//		// 检测突变
//		if((M3508[i].Angle - M3508[i].Last_Angle) < -4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//		// 已经一圈了
//		 	M3508[i].Round = M3508[i].Motor_Length;
//			angle_init[i]=0;
//		}
//		else if((M3508[i].Angle - M3508[i].Last_Angle) > 4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
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
//	// 速度积分
//	M3508[0].Motor_Length += M3508[0].Speed * 0.01;
//}

/**
  * @brief  电机位置控制
  * @param  Goal_Position:目标位置
  *         Motor_ID:电机电调(1, 2, 3, 4, 5, 6)
  * @retval 1:达到目标位置,0:未达到目标位置
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
//			// 速度的正负号需要调
//			Motor_Speed_Control(Set_speed, Motor_ID);
//		}
//		else{
//			// 速度的正负号需要调
//			Motor_Speed_Control(-Set_speed , Motor_ID);
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
//	return Return_Flag;
//}

/**
  * @brief  电机保护
  * @param  void
  * @retval 1:启动保护,0:未启动保护
  */

u8 Motor_Protect(void) {
	static u8 Stop_Count = 0;
	u8 Protect_Flag = 0;
	// 保护检测(条件)
	if ((fabs(M3508[0].Speed) < 100) || (fabs(M3508[1].Speed) < 100))
	{
		if ((fabs(M3508[0].Control_Current) > 2000) || (fabs(M3508[1].Control_Current) > 2000))
		{
			Stop_Count++;
		}
	}
	if (Stop_Count > 10)
	{
		Protect_Flag = 1;
		M3508[0].Start_Flag = 0;
		M3508[1].Start_Flag = 1;
	}
	return Protect_Flag;
}


