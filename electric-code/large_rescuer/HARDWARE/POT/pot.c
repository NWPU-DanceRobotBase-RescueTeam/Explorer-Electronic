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
#include "iwdg.h"

/**
  * @brief  电位器电压采集,并转换为对应角度
  * @param  void
  * @retval void
  */

void get_pot_adc_angle(u8 ID)
{
	u16 adc = 0;
	float v = 0;
	switch(ID)
	{
	case 0:
		adc = Get_Adc_Average(ADC_Channel_4, 5);
		break;
	case 1:
		adc = Get_Adc_Average(ADC_Channel_5, 5);
		break;
	case 2:
		adc = Get_Adc_Average(ADC_Channel_6, 5);
		break;
	case 3:
		adc = Get_Adc_Average(ADC_Channel_7, 5);
		break;
	}
	v = (float)(adc) * (3.3 / 4096);
	M3508[ID].Current_angle = v * 290 / 3.3f;
//	u16 adc[4] = { 0,0,0,0 };
//	float v[4] = { 0 };
//	u8 i;
//	adc[0] = Get_Adc_Average(ADC_Channel_4, 5);
//	adc[1] = Get_Adc_Average(ADC_Channel_5, 5);
//	adc[2] = Get_Adc_Average(ADC_Channel_6, 5);
//	adc[3] = Get_Adc_Average(ADC_Channel_7, 5);
//	for (i = 0; i < 4; i++)
//	{
//		v[i] = (float)(adc[i]) * (3.3 / 4096);
//		adc[i] = v[i];
//		M3508[i].Current_angle = v[i] * 290 / 3.3f;
//	}
}
/**
  * @brief  副履带复位
  * @param  void
  * @retval void
  */

void Subtrack_Rest(void)
{
	u8 i;
	for (i = 0; i < 4; i++)
	{
		get_pot_adc_angle(i);
	}
	M3508[0].PID.Goal_Position = Left_Front;//左前初始化角度
	M3508[1].PID.Goal_Position = Left_Rear;//左后初始化角度
	M3508[2].PID.Goal_Position = Right_Front;//右前初始化角度
	M3508[3].PID.Goal_Position = Right_Rear;//右后初始化角度
	for (i = 0; i < 4; i++)
	{
		M3508[i].Start_Flag = 1;
	}
	printf("reset\r\n");
}
/**
  * @brief  副履带位置控制
  * @param  void
  * @retval void
  */

void Angle_Position_Control(void)
{
	u8 i;
	s16 goal_speed = 0;
	for (i = 0; i < 4; i++)
	{
		if (M3508[i].Start_Flag == 1)  //未达到目标位置
		{
			get_pot_adc_angle(i);
			if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 8)  //恒定速度区
				goal_speed = 6000;     // 与软件速度同步  缓冲8度
			else if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 0.5)//速度缓冲区
				goal_speed = fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) * 150 + 750;
			else     //达到目标位置
				goal_speed = 0;

			// 电机1、4和2、3的安装方向相反
			if (M3508[i].Current_angle > M3508[i].PID.Goal_Position)
				goal_speed = -goal_speed;
			else
				goal_speed = goal_speed;

			if (i == 1 || i == 2)
				goal_speed = -goal_speed;
			else
				goal_speed = goal_speed;

			if (goal_speed == 0)
				M3508[i].Start_Flag = 0;  //停止
			else
				M3508[i].Start_Flag = 1;  //继续动

			M3508[i].PID.Goal_Speed = goal_speed;
			
			IWDG_Feed();
		}
	}
}

