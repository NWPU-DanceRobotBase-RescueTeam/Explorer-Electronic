/**************************************************************************
 作者：栋次大次、瑞瑞
 没有特别幸运，请先特别努力
 **************************************************************************/
#include "pot.h"

/**
 * @brief  电位器电压采集,并转换为对应角度
 * @param  void
 * @retval void
 */

void get_pot_adc_angle(uint8_t ID)
{
	uint16_t adc = 0;
	switch (ID)
	{
		case 0:
			adc = adc_buf[1];
			break;
		case 1:
			adc = adc_buf[3];
			break;
		case 2:
			adc = adc_buf[2];
			break;
		case 3:
			adc = adc_buf[0];
			break;
	}
	float v = (float) (adc) * (3.3 / 4096);
	M3508[ID].Current_angle = v * 290 / 3.3f;
}
/**
 * @brief  副履带复位
 * @param  void
 * @retval void
 */

void Subtrack_Rest(void)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		get_pot_adc_angle(i);
	}
	M3508[0].PID.Goal_Position = Left_Front + 35; //左前初始化角度
	M3508[1].PID.Goal_Position = Left_Rear - 35; //左后初始化角度
	M3508[2].PID.Goal_Position = Right_Front - 35; //右前初始化角度
	M3508[3].PID.Goal_Position = Right_Rear + 35; //右后初始化角度
	for (uint8_t i = 0; i < 4; i++)
	{
		M3508[i].Stop_Flag = 0;
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
	int16_t goal_speed = 0;
	for (uint8_t i = 0; i < 4; i++)
	{
		if (M3508[i].Stop_Flag < 5)  //未达到目标位置
		{
			get_pot_adc_angle(i);
			if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 8)  //恒定速度区
				goal_speed = Max_Speed;     // 与软件速度同步  缓冲8度
			else if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 0.5)     //速度缓冲区
				goal_speed = fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) * 200 + 1500;
			else
				//达到目标位置
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

			if (goal_speed == 0 && M3508[i].Stop_Flag < 10)
				M3508[i].Stop_Flag++;  //停止
			else
				M3508[i].Stop_Flag = 0;  //继续动

			M3508[i].PID.Goal_Speed = goal_speed;

//			HAL_IWDG_Refresh(&hiwdg);   //看门狗喂狗
		}
	}
}

