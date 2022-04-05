#include "debug.h"

/**
 * @brief  �������ٶ����趨�ٶ�
 * @param  void
 * @retval void
 */

void Report_Current(void)
{
	printf("�趨�ٶ�Ϊ��");
	for (uint8_t i = 0; i < 4; i++)
	{
		printf("%d ", M3508[i].PID.Goal_Speed);
	}
	printf("\r\n");
	printf("ʵ���ٶ�Ϊ��");
	for (uint8_t i = 0; i < 4; i++)
	{
		printf("%d ", M3508[i].Speed);
	}
	printf("\r\n");
	printf("\r\n");
}

/**
 * @brief  ���渱�Ĵ��Ƕ�
 * @param  void
 * @retval void
 */

void Report_ADC(void)
{
	printf("��ǰ�Ƕ�= %f\r\n", M3508[0].Current_angle);
	printf("���Ƕ�= %f\r\n", M3508[1].Current_angle);
	printf("��ǰ�Ƕ�= %f\r\n", M3508[2].Current_angle);
	printf("�Һ�Ƕ�= %f\r\n", M3508[3].Current_angle);
	printf("��ǰĿ��= %d\r\n", M3508[0].PID.Goal_Position);
	printf("���Ŀ��= %d\r\n", M3508[1].PID.Goal_Position);
	printf("��ǰĿ��= %d\r\n", M3508[2].PID.Goal_Position);
	printf("�Һ�Ŀ�� %d\r\n", M3508[3].PID.Goal_Position);
	printf("\r\n");
}

/**
 * @brief  �ٿ�
 * @param  void
 * @retval void
 */

void debug(void)
{
	uint8_t usart_receive_message[20];
	uint8_t receive_length = Usart_Receivecheck();
	if (receive_length)
	{
		printf("debug");
		uint8_t mode = 0;
		uint8_t message_length = FSM(receive_length, usart_receive_message, &mode);
		switch (mode)
		{
			case 0XFF:                          //���Ĵ�  ���400
				MAXON_MOTOR[0] = (usart_receive_message[1] - 100) * 100 + (usart_receive_message[0] - 100) * 100;
				MAXON_MOTOR[1] = -((usart_receive_message[1] - 100) * 100 - (usart_receive_message[0] - 100) * 100);
				printf("��:%d", MAXON_MOTOR[0]);
				printf("��:%d", MAXON_MOTOR[1]);
				break;

			case 0XFB:							//PID����
				if (message_length == 7)
				{
					for (uint8_t i = 0; i < 7; i++)
					{
						M3508[i].PID.Kp = usart_receive_message[0] / 10;
						M3508[i].PID.Ki = usart_receive_message[1] / 10;
						M3508[i].PID.Kd = usart_receive_message[2] / 10;
						M3508[i].Current_Limit = ((usart_receive_message[3] << 8) + usart_receive_message[4]);
						M3508[i].PID.Error_Sum_Limit = ((usart_receive_message[5] << 8) + usart_receive_message[6]);
					}
					printf("PID���óɹ���\r\n");
				}
				else
				{
					printf("δ�ܳɹ�����PID��\r\n");
				}
				break;

			case 0XFA:												//�趨��λ��
				for (uint8_t i = 0; i < message_length; i = i + 2)
				{
					//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
					if (i == 0)
						M3508[0].PID.Goal_Position = Left_Front - ((usart_receive_message[i] << 8) + usart_receive_message[i + 1]) * 1.1111;
					else if (i == 2)
						M3508[1].PID.Goal_Position = Left_Rear + ((usart_receive_message[i] << 8) + usart_receive_message[i + 1]) * 1.1111;
					else if (i == 4)
						M3508[2].PID.Goal_Position = Right_Front + ((usart_receive_message[i] << 8) + usart_receive_message[i + 1]) * 1.1111;
					else if (i == 6)
						M3508[3].PID.Goal_Position = Right_Rear - ((usart_receive_message[i] << 8) + usart_receive_message[i + 1]) * 1.1111;
					printf("λ�ýǶ�����Ϊ��%d\r\n", M3508[i / 2].PID.Goal_Position);
					M3508[i / 2].Stop_Flag = 0;
				}
				break;

			case 0XF9:
				HAL_GPIO_TogglePin(REALY_GPIO_Port, REALY_Pin);
				break;

			default:
				break;
		}
		receive_length = 0;
	}
}

