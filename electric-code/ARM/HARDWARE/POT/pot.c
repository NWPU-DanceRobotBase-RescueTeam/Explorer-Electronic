/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/
#include <math.h>
#include "pot.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "Motor_Control.h"

const float SECOND_ARM_START = 290.0f;

float second_arm_goal_postion;
float first_arm_goal_position;

/**
  * @brief  电位器电压采集,并转换为对应角度
  * @param  void
  * @retval void
  */
  
void get_pot_adc_angle()
{
    u16 adc = 0;
    float v = 0;
    adc = Get_Adc_Average(ADC_Channel_2, 3);
    v = (float)(adc) * (3.3 / 4096);
    Motor[0].Current_angle = v * 350 / 3.3f;  //有效角度350
}

/**
  * @brief  二级臂复位
  * @param  void
  * @retval void
  */

void Arm_Rest()
{
    get_pot_adc_angle();
    second_arm_goal_postion = 0.0f;
    Motor[0].Start_Flag = 1;
}

/**
  * @brief  二级臂位置控制
  * @param  Motor_ID:电机电调
  * @retval void
  */

void Angle_Position_Control(u8 Motor_ID)
{
//	static u8 i = 0;
	static s16 max_speed = 150;
	s16 goal_speed = 0, Kp = 25;
	static u8 stop = 0; 
	float error = SECOND_ARM_START - second_arm_goal_postion - Motor[Motor_ID].Current_angle;
//	static u8 times = 0, times_error = 0;
	
//	static float last_error = 0, last_error2 = 0;
//	static s16 last_speed = 0, last_speed2 = 0;
	
	if (fabs(error) > 5)
		goal_speed = max_speed;
	else if (fabs(error) > 0.5)
		goal_speed = fabs(error) * Kp;
	else
		goal_speed = 0;

	if (error < 0)
		goal_speed = -goal_speed;
	else
		goal_speed = goal_speed;

	if (goal_speed == 0)
	{
		stop++;
	}
	else
	{
		Motor[Motor_ID].Start_Flag = 1;  //继续动
		stop = 0;
	}

	if (stop >= 5)
	{
		Motor[Motor_ID].Start_Flag = 0;  //停止
		Motor[Motor_ID].PID.Error_Sum = 0;
	}
	
//	if ( fabs(error) - fabs(last_error) > 2 || fabs(error) - fabs(last_error2) > 2 )
//	{
//		times = 0;
//		if (times_error++ > 3)
//			max_speed -= 10;
//		printf("!\r\n");
//	}
//	else
//	{
//		times_error = 0;
//		if(times++ > 5)
//			max_speed = 85;
//	}
	
//	if ( (fabs(goal_speed) >= max_speed - 20 && fabs(last_speed) >= max_speed - 20) || (fabs(goal_speed) >= max_speed - 20 && fabs(last_speed2) >= max_speed - 20) )   // 防止振动不断变大
//	{
//		times = 0;
//		if (times_error++ > 5)
//			max_speed -= 10;
//		if (max_speed >= 60)
//			max_speed -= 10;
//		if (goal_speed > 0)
//			goal_speed -= 20;
//		else
//			goal_speed += 20;
//		
//		Motor[0].PID.Error_Sum = 0;
//		last_speed2 = 0;
//		last_speed = 0;
//		
//		printf("!\r\n");
//	}
//	else
//	{
//		times_error = 0;
//		if(times++ > 5)
//			max_speed = 100;
//	}
	
	Motor[Motor_ID].PID.Goal_Speed = goal_speed;
	
//	last_error = error;
//	last_error2 = last_error;
//	last_speed = goal_speed;
//	last_speed2 = last_speed;

//	if(i++ > 10)
//	{
//		printf("max speed:%d\r\n", max_speed);
//		i = 0;
//	}
}
