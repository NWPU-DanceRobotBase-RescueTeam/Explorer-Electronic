/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/
#include <math.h>
#include "delay.h"
#include "Motor_Control.h"
#include "pot.h"
#include "arm.h"
#include "usart.h"
#include "can.h"

struct MOTOR_T Motor[1];

/**
  * @brief  M3508结构体初始化
  * @param  void
  * @retval void
  */
  
void Control_Initialize(void)
{
	// 电调1（二级臂）M3508
	{
		// 电机机械角
		Motor[0].Angle = 0;
		// 电机扭矩
		Motor[0].Moment = 0;
		// 电机当前速度
		Motor[0].Speed = 0;
		// 电机电流输出量
		Motor[0].Control_Current = 0;
//		Motor[0].Last_Control_Current=0;
//		Motor[0].Stop_On = 0;
		// 电机停止标志
		Motor[0].Start_Flag = 1;
		// 电机电流限幅值(绝对值)
		Motor[0].Current_Limit = 4000;
//		Motor[0].Round = 0;
//		Motor[0].Motor_Length = 0;
//		Motor[0].Last_Angle = 0;
		// PID结构体初始化
		Motor[0].PID.Kp = 10;    //100
		Motor[0].PID.Ki = 0.2;      //3
		Motor[0].PID.Kd = 0.4;
		Motor[0].PID.Kp2 = 20;
		Motor[0].PID.Ki2 = 0.4;
		Motor[0].PID.Kd2 = 0.8;
		Motor[0].PID.Last_Error = 0;
		Motor[0].PID.Goal_Speed = 0;
		Motor[0].PID.Error_Sum = 0;
	}
//	// 电调2（小臂）	M3508
//	{
//		Motor[1].Angle = 0;
//		Motor[1].Moment = 0;
//		Motor[1].Speed = 0;
//		Motor[1].Control_Current = 0;
//		Motor[1].Last_Control_Current=0;
//		Motor[1].Stop_On = 0;
//		Motor[1].Stop_Flag = 0;
//		Motor[1].Current_Limit = 15000;
//		Motor[1].Round = 0;
//		Motor[1].Motor_Length = 0;
//		Motor[1].Last_Angle = 0;
//		// PID结构体初始化
//		Motor[1].PID.Kp = 8;
//		Motor[1].PID.Ki = 0.6;
//		Motor[1].PID.Kd = 0;
//		Motor[1].PID.Last_Error = 0;
//		Motor[1].PID.Goal_Speed = 0;
//		Motor[1].PID.Error_Sum = 0;
//	}
//		// 电调3（底座）	M2006
//	{
//		Motor[2].Angle = 0;
//		Motor[2].Moment = 0;
//		Motor[2].Speed = 0;
//		Motor[2].Control_Current = 0;
//		Motor[2].Last_Control_Current=0;
//		Motor[2].Stop_On = 0;
//		Motor[2].Stop_Flag = 0;
//		Motor[2].Current_Limit = 10000;
//		Motor[2].Round = 0;
//		Motor[2].Motor_Length = 0;
//		Motor[2].Last_Angle = 0;
//		// PID结构体初始化
//		Motor[2].PID.Kp = 8;				//M2006的PID待调节
//		Motor[2].PID.Ki = 0.6;
//		Motor[2].PID.Kd = 0;
//		Motor[2].PID.Last_Error = 0;
//		Motor[2].PID.Goal_Speed = 0;
//		Motor[2].PID.Error_Sum = 0;
//	}
	
}

/**
  * @brief  将M3508回传数据转换为物理值
  * @param  电机电调(0)
  * @retval void
  */

void Data_Convert(u8 Motor_ID)
{
	Motor[Motor_ID].Angle = (int16_t)((Motor[Motor_ID].Original_Data[0] << 8) + Motor[Motor_ID].Original_Data[1]);
	Motor[Motor_ID].Speed = (int16_t)((Motor[Motor_ID].Original_Data[2] << 8) + Motor[Motor_ID].Original_Data[3]);
	Motor[Motor_ID].Moment = (int16_t)((Motor[Motor_ID].Original_Data[4] << 8) + Motor[Motor_ID].Original_Data[5]);
}

/**
  * @brief  通过CAN2发送控制量给电机
  * @param  void
  * @retval void
  */

void CAN_Send_Control_Value(void)		//CAN2给电机的发送函数
{
//	u8 i;
	static u8 Motor_Current[2] = {0, 0};
//	static u8 First_Flag = 1;
/*	if(First_Flag){			//只执行一次（初始化）
		for(i = 0; i < 6; ++i)
		{
			Motor_Current[i] = 0;
		}
		First_Flag = 0;
	} */
								//应该不需要，static初始化即使不写出也为0，之后也会随数据变化
	// 填入CAN包
	Motor_Current[0] = (u8)(((Motor[0].Control_Current) & 0xFF00) >> 8);			//电调1 M3508
	Motor_Current[1] = (u8)((Motor[0].Control_Current) & 0x00FF);
	
//	Motor_Current[2] = (u8)(((Motor[1].Control_Current) & 0xFF00) >> 8);			//电调2 M3508
//	Motor_Current[3] = (u8)((Motor[1].Control_Current) & 0x00FF);
//	
//	Motor_Current[4] = (u8)(((Motor[2].Control_Current) & 0xFF00) >> 8);			//电调3 M2006
//	Motor_Current[5] = (u8)((Motor[2].Control_Current) & 0x00FF);
	// 电调 1 ~ 4 	0x200
//	CAN2_Send_Msg(Motor_Current, 6, 0x200);		//发送控制电流值（根据C620及C610电调的接收报文格式）
	CAN2_Send_Msg(Motor_Current, 2, 0x200);		//发送控制电流值（根据C620及C610电调的接收报文格式）
//	printf("CAN\r\n");
//	printf("%x %x\r\n",Motor_Current[4],Motor_Current[5]);
}


/**
  * @brief  电机速度控制函数(PID)(内部带积分限幅)  位置式
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  *         Goal_Positiom:目标速度
  * @retval void
  */

void Motor_Speed_Control(s32 Goal_Speed, u8 Motor_ID)
{
	float Error;
	float Control_Current = 0;
	static u8 i = 0;
	
	//计算角度
//	float seita = -first_arm_goal_position;
//	float fai = (SECOND_ARM_START - Motor[Motor_ID].Current_angle) / MOTOR_RAD;
//	float angle = seita - fai;
//	float rest = -cos(angle) * 1600;

	// 计算偏差
	Error = Goal_Speed - Motor[Motor_ID].Speed;
	if (Error > 200)
	{
		Error = 200;
	}
	else if (Error < -200)
	{
		Error = -200;
	}

	Motor[Motor_ID].PID.Error_Sum += Error;
	// PID积分限幅
	if (fabs(Motor[Motor_ID].PID.Error_Sum) > 10000)
	{
		if (Motor[Motor_ID].PID.Error_Sum > 0)
			Motor[Motor_ID].PID.Error_Sum = 10000;
		else
			Motor[Motor_ID].PID.Error_Sum = -10000;
	}
	// 计算电流输出量
	if(Goal_Speed > 0)
	{		
		Control_Current = Motor[Motor_ID].PID.Kp * Error +
						  Motor[Motor_ID].PID.Ki * Motor[Motor_ID].PID.Error_Sum +
						  Motor[Motor_ID].PID.Kd * (Error - Motor[Motor_ID].PID.Last_Error);
	}
	else
	{
		Control_Current = Motor[Motor_ID].PID.Kp2 * Error +
						  Motor[Motor_ID].PID.Ki2 * Motor[Motor_ID].PID.Error_Sum +
						  Motor[Motor_ID].PID.Kd2 * (Error - Motor[Motor_ID].PID.Last_Error);
	}
	
	Motor[Motor_ID].PID.Last_Error = Error;
	// 电流限幅
	if (fabs(Control_Current) < 10)
		Control_Current = 0;
	else 
	{
		// 限幅
		if (fabs(Control_Current) > Motor[Motor_ID].Current_Limit) 
		{
			if (Control_Current > 0)
				Control_Current = Motor[Motor_ID].Current_Limit;
			else 
				Control_Current = -Motor[Motor_ID].Current_Limit;
		}
		else 
		{
			Control_Current = (s16)(Control_Current);
		}
	}
	
//		Motor[Motor_ID].Control_Current = rest + Control_Current;
	Motor[Motor_ID].Control_Current = Control_Current;
	
	if(i++ > 10)
	{
		printf("%d  %d  %.2f  %d\r\n", Motor[0].PID.Goal_Speed, Motor[Motor_ID].Speed, Control_Current, Motor[Motor_ID].Speed);
		i = 0;
	}
}

/**
  * @brief  电机位置控制函数(PID)(内部带积分限幅)  位置式
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  *         Goal_Position:目标位置
  * @retval void
  */

void Motor_Position_Control(s32 Goal_Position, u8 Motor_ID)
{
//	static float last_current = 0;
//	static u8 i = 0;
	float Error;
	float Control_Current = 0;
	
	//计算角度
	float seita = -first_arm_goal_position;
	float fai = (SECOND_ARM_START - Motor[Motor_ID].Current_angle) / MOTOR_RAD;
	float angle = seita - fai;
	float rest = -cos(angle) * 1300;
	
	// 计算偏差
	Error = Goal_Position - Motor[Motor_ID].Current_angle;
	if (Error > 180)
	{
		Error = 180;
	}
	else if (Error < -180)
	{
		Error = -180;
	}

	// PID积分限幅
	if (fabs(Motor[Motor_ID].PID.Error_Sum) > 5000)
	{
		if (Motor[Motor_ID].PID.Error_Sum > 0)
			Motor[Motor_ID].PID.Error_Sum = 5000;
		else
			Motor[Motor_ID].PID.Error_Sum = -5000;
	}
	// 计算电流输出量
	if(fabs(rest) <= 500)
	{		
		Control_Current = rest + Motor[Motor_ID].PID.Kp2 * Error +
						  Motor[Motor_ID].PID.Ki2 * Motor[Motor_ID].PID.Error_Sum +
						  Motor[Motor_ID].PID.Kd2 * (Error - Motor[Motor_ID].PID.Last_Error);
	}
	else if (fabs(Error) > 20.0f)
	{
		if(Error > 0)
		{
			Control_Current = rest + 2000;
		}
		else
		{
			Control_Current = rest - 2000;
		}
	}
	else if (fabs(Error) > 0.5f)
	{
		Control_Current = rest + Motor[Motor_ID].PID.Kp * Error +
						  Motor[Motor_ID].PID.Ki * Motor[Motor_ID].PID.Error_Sum +
						  Motor[Motor_ID].PID.Kd * (Error - Motor[Motor_ID].PID.Last_Error);
	}
	else
	{
//		Control_Current = Motor[Motor_ID].PID.Ki2 * Motor[Motor_ID].PID.Error_Sum +
//						  Motor[Motor_ID].PID.Kd2 * (Error - Motor[Motor_ID].PID.Last_Error);
//		if(Error > 0)
//		{
//			Control_Current = rest +
//							  Motor[Motor_ID].PID.Ki2 * Motor[Motor_ID].PID.Error_Sum +
//							  Motor[Motor_ID].PID.Kd2 * (Error - Motor[Motor_ID].PID.Last_Error);
//		}
//		else
//		{
//			Control_Current = rest +
//							  Motor[Motor_ID].PID.Ki2 * Motor[Motor_ID].PID.Error_Sum +
//							  Motor[Motor_ID].PID.Kd2 * (Error - Motor[Motor_ID].PID.Last_Error);
//		}
		Control_Current = rest;
		Motor[Motor_ID].PID.Error_Sum = 0;
		
	}
	Motor[Motor_ID].PID.Error_Sum += Error;
	
	Motor[Motor_ID].PID.Last_Error = Error;
		// 电流限幅
	if (fabs(Control_Current) < 1) 
		Control_Current = 0;
	else 
	{
		// 限幅
		if (fabs(Control_Current) > Motor[Motor_ID].Current_Limit) 
		{
			if (Control_Current > 0) 
				Control_Current = Motor[Motor_ID].Current_Limit;
			else 
				Control_Current = -Motor[Motor_ID].Current_Limit;
		}
		else
		{
			Control_Current = (s16)(Control_Current);
		}
	}
	Motor[Motor_ID].Control_Current = Control_Current;
	
//	if(i++ > 50)
//	{
//		printf("%f %f rest: %f\r\n", seita, fai, rest);
//		i = 0;
//	}
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
//	// 计算电流输出量																									 //		第二个              第三个
//	Increment_Control_Current = M3508[Motor_ID].PID.Kp *( M3508[Motor_ID].PID.error-M3508[Motor_ID].PID.Next_Error)+	//Next为第k-1个error，Last为第k-2个error
//										M3508[Motor_ID].PID.Ki * M3508[Motor_ID].PID.error + 
//										M3508[Motor_ID].PID.Kd * (M3508[Motor_ID].PID.error- 2*M3508[Motor_ID].PID.Next_Error + M3508[Motor_ID].PID.Last_Error);
//	//M3508[Motor_ID].PID.Last_Error = Error;
//	M3508[Motor_ID].Control_Current =M3508[Motor_ID].Control_Current+(int16_t)Increment_Control_Current;
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
//	if(fabs(M3508[Motor_ID].Control_Current ) < 1) M3508[Motor_ID].Control_Current  = 0;
//	else{
//		// 限幅
//		if(fabs(M3508[Motor_ID].Control_Current ) > M3508[Motor_ID].Current_Limit){
//			if(M3508[Motor_ID].Control_Current  > 0) M3508[Motor_ID].Control_Current  = M3508[Motor_ID].Current_Limit;
//			else M3508[Motor_ID].Control_Current  = -M3508[Motor_ID].Current_Limit;
//		}
//	}
//}

/**
  * @brief  电机路程计算
  * @param  电机电调(1, 2, 3, 4, 5, 6)
  * @retval void
  * @note   此函数需要优化(根据速度进行优化)
  */
 
//void Motor_Length_Calc(void)
//{
//	u8 i;
//	static u8 First_Flag = 1;
//	static int angle_init = 0;
//	if(First_Flag)		//只执行一次，初始化操作
//	{
//		angle_init= Motor[0].Angle;
//		Motor[0].Last_Angle=Motor[0].Angle;
//		First_Flag = 0;
//	}
//	for(i = 0; i < 3; ++i)
//	{
//		// 检测突变
//		if((Motor[i].Angle - Motor[i].Last_Angle) < -4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//		//	// 已经一圈了
//		 	Motor[i].Round = Motor[i].Motor_Length;
//			angle_init=0;
//		}
//		else if((Motor[i].Angle - Motor[i].Last_Angle) > 4000)
//		{
//		//	M3508[i].Last_Angle = M3508[i].Angle;
//			// 已经一圈了
//			Motor[i].Round = Motor[i].Motor_Length;
//			angle_init=8192;
//		}
//		if(Motor[i].Speed==0) 
//		{
//			//M3508[i].Motor_Length=0;
//			//angle_init=M3508[i].Angle;
//		}
//		else Motor[i].Motor_Length = Motor[i].Round +(Motor[i].Angle-angle_init)*0.043945;	//计算路程//Round为转的超过一圈时之前得到的路程（角度），Length为电机转过的度数
//		Motor[i].Last_Angle=Motor[i].Angle;
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

//void motor_set(u8 id_uart_receive,int pwm_uart_receive)//电机速度控制（根据位置）
//{
//    static u16 position_last[2]={0,0};
//	s16 position_goal=0;
//	
//	position_goal=-pwm_uart_receive;
//	
//	if(position_goal>(Motor[id_uart_receive-1].Motor_Length+100))	//还未达到目标位置，以一恒定速度进行运动
//		Motor[id_uart_receive-1].PID.Goal_Speed=100;
//	
//	else if(position_goal<(Motor[id_uart_receive-1].Motor_Length-100)) 
//		Motor[id_uart_receive-1].PID.Goal_Speed=-100;
//	
//	else 
//		Motor[id_uart_receive-1].PID.Goal_Speed=0;			//基本达到目标位置，进行减速，到零
//	
//	while(1)			//超出目标位置，进行减速，到零
//	{      
//	  if(Motor[id_uart_receive-1].PID.Goal_Speed>0 && Motor[id_uart_receive-1].Motor_Length >= (position_goal+20) )	
//		{	
//			Motor[id_uart_receive-1].PID.Goal_Speed=0;					
//			//M3508[0].Motor_Length=0;
////			delay_ms(50);
//			
//			//M3508[0].Round=0;
//			position_last[id_uart_receive-1] = Motor[id_uart_receive-1].Motor_Length;
//			break;
//		}
//		else if(Motor[id_uart_receive-1].PID.Goal_Speed<0 && Motor[id_uart_receive-1].Motor_Length <=( position_goal+60) )
//		{	
//			Motor[id_uart_receive-1].PID.Goal_Speed=0;
//			
////			delay_ms(50);
//			
//			//M3508[0].Round=0;
//			position_last[id_uart_receive-1] = Motor[id_uart_receive-1].Motor_Length;
//			break;
//		}
//		else if(Motor[id_uart_receive-1].PID.Goal_Speed==0)
//		{
//			break;
//		}
//	}

//}
