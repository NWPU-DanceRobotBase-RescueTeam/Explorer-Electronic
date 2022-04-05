#include <string.h>
#include "debug.h"
#include "usart.h"
#include "delay.h"
#include "arm.h"
#include "led.h"
#include "GIM8008.h"
#include "fsm.h"
#include "pot.h"
#include "Motor_Control.h"

/**
  * @brief  ²Ù¿Ø
  * @param  void
  * @retval void
  */

void debug(void)
{
	static s16 base_receive = 0, pwm4_receive = 0, pwm5_receive = 0, pwm6_receive = 0, pwm7_receive = 0;
//	static float gim_receive = 0;
	s16 int_temp = 0;
	float float_temp = 0;
	float radian = 0;
	//	static s16 pwm_receive=0;
	//	static float gim_recive=0;
	u8 receive_length = 0, mode = 0;
	//	float radian=0;
	u8 length = 0;
	u8 usart_receive_message[20];
	receive_length = Usart_Receivecheck();
	if (receive_length)
	{
		length = FSM(receive_length, usart_receive_message, &mode);
		if(length != 0)
		{
			switch (mode)
			{
			case 0x00:
	//			EXTI_GenerateSWInterrupt(EXTI_Line0);
				break;

			case 0x05:
				memcpy(&radian, usart_receive_message, 4);		// 0 - 1.57 - 3.14
				int_temp = (int)(radian * SERVO_RAD1 + 500);
				if (int_temp <= 500)
					int_temp = 500;
				else if (int_temp >= 2500)
					int_temp = 2500;
				if (int_temp != base_receive)
				{
					base_receive = int_temp;
					servo_set(ID3, base_receive);
				}
				printf("3:%d\r\n", base_receive);
				break;

			case 0x16:
				memcpy(&radian, usart_receive_message, 4);   // 0 - 1.5 - 3
				float_temp = -radian;             				// 0 - 2.16
				if (float_temp <= -4)
					float_temp = -4;
				else if (float_temp >= 0)
					float_temp = 0;
				if (float_temp != first_arm_goal_position)
				{
					first_arm_goal_position = float_temp;
					Send_8008_Control((first_arm_goal_position+0.2f)*0.83f, 0, GIM_Kp, GIM_Kd, 0);
				}
				printf("1:%.2f\r\n", first_arm_goal_position);
//				if(pwm_receive!=motor1pwm_receive)
//				{
//					motor1pwm_receive=pwm_receive;
//					motor_set(ID1,motor1pwm_receive);
//				}

				memcpy(&radian, usart_receive_message + 4, 4);  // 0 - 3.14
				float_temp = (float)(radian * MOTOR_RAD);
				if (float_temp >= 270)
					float_temp = 270.0f;
				else if (float_temp <= 0)
					float_temp = 0;
				if (float_temp != second_arm_goal_postion)
				{
					second_arm_goal_postion = float_temp;
					Motor[0].Start_Flag = 1;
				}
				printf("2:%.2f\r\n", second_arm_goal_postion);
				break;

			case 0x27:
				memcpy(&radian, usart_receive_message, 4);  // 0 - 3.14
				int_temp = (int)(radian * SERVO_RAD + 500);
				if (int_temp <= 500)
					int_temp = 500;
				else if (int_temp >= 2500)
					int_temp = 2500;
				if (int_temp != pwm4_receive)
				{
					pwm4_receive = int_temp;
					servo_set(ID4, pwm4_receive);
				}
				printf("4:%d\r\n", pwm4_receive);

				memcpy(&radian, usart_receive_message + 4, 4); // 0.785 - 1.09 - 2.9
				int_temp = (int)(radian * SERVO_RAD + 500);
				if (int_temp <= 1000)
					int_temp = 1000;
				else if (int_temp >= 2350)
					int_temp = 2350;
				if (int_temp != pwm5_receive)
				{
					pwm5_receive = int_temp;
					servo_set(ID5, pwm5_receive);
				}
				printf("5:%d\r\n", pwm5_receive);
				break;

			case 0x38:
				memcpy(&radian, usart_receive_message, 4);
				int_temp = (int)(radian * SERVO_RAD + 500);
				if (int_temp <= 1450)
					int_temp = 1450;
				else if (int_temp >= 2150)
					int_temp = 2150;
				if (int_temp != pwm7_receive)
				{
					pwm7_receive = int_temp;
					servo_set(ID7, pwm7_receive);
				}
				printf("7:%d\r\n", pwm7_receive);

				memcpy(&radian, usart_receive_message + 4, 4); //  0 - 1.57 - 3.14
				int_temp = (int)(radian * SERVO_RAD + 500);
				if (int_temp <= 500)
					int_temp = 500;
				else if (int_temp >= 2500)
					int_temp = 2500;
				if (int_temp != pwm6_receive)
				{
					pwm6_receive = int_temp;
					servo_set(ID6, pwm6_receive);
				}
				printf("6:%d\r\n", pwm6_receive);
				break;

			case 0x49:
				EXTI_GenerateSWInterrupt(EXTI_Line1);
				break;

			case 0x5a:

				break;

			default:
				break;
			}
		}
		//		LED0=~LED0;
		receive_length = 0;
	}
}

