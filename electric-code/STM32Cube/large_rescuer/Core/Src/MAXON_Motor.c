#include "MAXON_Motor.h"

/**
  * @brief  MAXON速度设置
  * @param  void
  * @retval void
  */

int16_t MAXON_MOTOR[2]; //MAXON电机速度

void Maxon_Speed_Control(void)  // pwm值：10Hz--5kHz
{
	uint16_t pwm = 0;

	pwm = 50 + fabs(MAXON_MOTOR[0]) / 10;

	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[0] >= 0)
		HAL_GPIO_WritePin(LEFT_GPIO_Port, LEFT_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LEFT_GPIO_Port, LEFT_Pin, GPIO_PIN_RESET);

	LEFT_Speed(pwm);

	pwm = 50 + fabs(MAXON_MOTOR[1]) / 10;
	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[1] >= 0)
		HAL_GPIO_WritePin(RIGHT_GPIO_Port, RIGHT_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(RIGHT_GPIO_Port, RIGHT_Pin, GPIO_PIN_RESET);

	RIGHT_Speed(pwm);
}

