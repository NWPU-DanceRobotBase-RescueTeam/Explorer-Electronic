/**************************************************************************
 ���ߣ����δ�Ρ�����
 û���ر����ˣ������ر�Ŭ��
 **************************************************************************/
#include "pot.h"

/**
 * @brief  ��λ����ѹ�ɼ�,��ת��Ϊ��Ӧ�Ƕ�
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
 * @brief  ���Ĵ���λ
 * @param  void
 * @retval void
 */

void Subtrack_Rest(void)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		get_pot_adc_angle(i);
	}
	M3508[0].PID.Goal_Position = Left_Front + 35; //��ǰ��ʼ���Ƕ�
	M3508[1].PID.Goal_Position = Left_Rear - 35; //����ʼ���Ƕ�
	M3508[2].PID.Goal_Position = Right_Front - 35; //��ǰ��ʼ���Ƕ�
	M3508[3].PID.Goal_Position = Right_Rear + 35; //�Һ��ʼ���Ƕ�
	for (uint8_t i = 0; i < 4; i++)
	{
		M3508[i].Stop_Flag = 0;
	}
	printf("reset\r\n");
}
/**
 * @brief  ���Ĵ�λ�ÿ���
 * @param  void
 * @retval void
 */

void Angle_Position_Control(void)
{
	int16_t goal_speed = 0;
	for (uint8_t i = 0; i < 4; i++)
	{
		if (M3508[i].Stop_Flag < 5)  //δ�ﵽĿ��λ��
		{
			get_pot_adc_angle(i);
			if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 8)  //�㶨�ٶ���
				goal_speed = Max_Speed;     // ������ٶ�ͬ��  ����8��
			else if (fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) > 0.5)     //�ٶȻ�����
				goal_speed = fabs(M3508[i].Current_angle - M3508[i].PID.Goal_Position) * 200 + 1500;
			else
				//�ﵽĿ��λ��
				goal_speed = 0;

			// ���1��4��2��3�İ�װ�����෴
			if (M3508[i].Current_angle > M3508[i].PID.Goal_Position)
				goal_speed = -goal_speed;
			else
				goal_speed = goal_speed;

			if (i == 1 || i == 2)
				goal_speed = -goal_speed;
			else
				goal_speed = goal_speed;

			if (goal_speed == 0 && M3508[i].Stop_Flag < 10)
				M3508[i].Stop_Flag++;  //ֹͣ
			else
				M3508[i].Stop_Flag = 0;  //������

			M3508[i].PID.Goal_Speed = goal_speed;

//			HAL_IWDG_Refresh(&hiwdg);   //���Ź�ι��
		}
	}
}

