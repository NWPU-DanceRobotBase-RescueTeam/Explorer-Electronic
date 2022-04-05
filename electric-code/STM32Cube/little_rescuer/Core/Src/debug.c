#include "debug.h"

//u32 PID_coefficient[5];

/**
 * @brief  ���͸�����ٶ�
 * @param  void
 * @retval void
 */

//void report()
//{
//	for (uint8_t i = 0; i < message_length / 2; i++)
//	{
//		printf("%d:%d ", i, M3508[i].Speed);
//	}
//	printf("\r\n");
//}

/**
 * @brief  ����PID
 * @param  void
 * @retval void
 */

//void PID_coefficient_Change(void)
//{
//	if (PID_coefficient[0] < 1000000)
//	{
//		for (i = 0; i < 4; i++)
//		{
//			M3508[i].PID.Kp = PID_coefficient[0] * 1.0 / 1000;
//			M3508[i].PID.Ki = PID_coefficient[1] * 1.0 / 1000;
//			M3508[i].PID.Kd = PID_coefficient[2] * 1.0 / 1000;
//			M3508[i].Current_Limit = PID_coefficient[3];
//		}
//		Error_Sum_set = PID_coefficient[4];
//		printf("\nKp=%.2f\r\n", M3508[0].PID.Kp);
//		printf("\nKi=%.2f\r\n", M3508[0].PID.Ki);
//		printf("\nKd=%.2f\r\n", M3508[0].PID.Kd);
//		printf("\nCurrent_Limit=%d\r\n", M3508[0].Current_Limit);
//		printf("\nError_Sum_set=%d\r\n", Error_Sum_set);
//	}
//}
/**
 * @brief  ����
 * @param  void
 * @retval void
 */

void debug()
{
	uint8_t usart_receive_message[20];
	uint8_t receive_length = 0, message_length, mode;
	receive_length = Usart_Receivecheck();
	if (receive_length)
	{
		message_length = FSM(receive_length, usart_receive_message, &mode);
		if (message_length != 0)
		{
			switch (mode)
			{
				case 0XFF:                          //����
					for (uint8_t i = 0; i < 4; i++)
					{
						//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
						if (i == 0 || i == 3)
							M3508[i].PID.Goal_Speed = (usart_receive_message[1] - 100) * 150 + (usart_receive_message[0] - 100) * 150;
						else
							M3508[i].PID.Goal_Speed = -((usart_receive_message[1] - 100) * 150 - (usart_receive_message[0] - 100) * 150);
					}
					//report();
					break;

//			case 0XFA:                          //ƽ��
//				for(i=0;i<4;i++)
//				{
//					//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//					if (i==0)M3508[i].PID.Goal_Speed=-((usart_receive_message[1]-100)*150+(usart_receive_message[0]-100)*150);
//					else if (i==3) M3508[i].PID.Goal_Speed=-((usart_receive_message[1]-100)*150-(usart_receive_message[0]-100)*150);
//					else if (i==1) M3508[i].PID.Goal_Speed=((usart_receive_message[1]-100)*150-(usart_receive_message[0]-100)*150);
//					else if (i==2) M3508[i].PID.Goal_Speed=-((usart_receive_message[1]-100)*150-(usart_receive_message[0]-100)*150);
//				}
//				//report();
//				break;
//					
//				case 0XFC:                          //б��
//				for(i=0;i<4;i++)
//				{
//					//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//					if (i==0)M3508[i].PID.Goal_Speed=(usart_receive_message[1]-100)*150+(usart_receive_message[0]-100)*150;	
//					else if (i==2) M3508[i].PID.Goal_Speed=((usart_receive_message[1]-100)*150-(usart_receive_message[0]-100)*150);
//				}
//				//report();
//				break;
//					
//				case 0XF0:                          //б��
//				for(i=0;i<4;i++)
//				{
//					//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//					if (i==1)M3508[i].PID.Goal_Speed=-((usart_receive_message[1]-100)*150+(usart_receive_message[0]-100)*150);	
//					else if (i==3) M3508[i].PID.Goal_Speed=-((usart_receive_message[1]-100)*150-(usart_receive_message[0]-100)*150);
//				}
//				//report();
//				break;

				case 0x01:
					refresh = 1;
					break;

				case 0x13:

					break;

				case 0x25:
					HAL_GPIO_TogglePin(HEADLIGHT_GPIO_Port, HEADLIGHT_Pin);
					break;

				case 0x37:
					HAL_GPIO_TogglePin(BUZZER_GPIO_Port,BUZZER_Pin);
					break;

//			case 0XFB:							//PID����
//				if (message_length == 10)
//				{
//					for (i = 0; i < 4; i++)
//					{
//						PID_coefficient[0] = ((usart_receive_message[0] << 8) + usart_receive_message[1]);
//						PID_coefficient[1] = ((usart_receive_message[2] << 8) + usart_receive_message[3]);
//						PID_coefficient[2] = ((usart_receive_message[4] << 8) + usart_receive_message[5]);
//						PID_coefficient[3] = ((usart_receive_message[6] << 8) + usart_receive_message[7]);
//						PID_coefficient[4] = ((usart_receive_message[8] << 8) + usart_receive_message[9]);
//					}
//					PID_coefficient_Change();
//					printf("PID���óɹ���\r\n");
//				}
//				else
//				{
//					printf("δ�ܳɹ�����PID��\r\n");
//				}
//				break;
			}
		}
		receive_length = 0;
	}
}

