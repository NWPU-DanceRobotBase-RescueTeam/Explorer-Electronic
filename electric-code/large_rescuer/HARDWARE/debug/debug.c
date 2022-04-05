#include "debug.h"
#include "usart.h"
#include "delay.h"
#include "fsm.h"
#include "Motor_Control.h"
#include "MAXON_Motor.h"
#include "tim.h"
#include "adc.h"
#include "pot.h"
#include "led.h"

/**
  * @brief  �������ٶ����趨�ٶ�
  * @param  void
  * @retval void
  */
 
void Report_Current(void)
{
	u8 i;
	printf("�趨�ٶ�Ϊ��");
	for (i = 0; i < 4; i++)
	{
		printf("%d ", M3508[i].PID.Goal_Speed);
	}
	printf("\r\n");
	printf("ʵ���ٶ�Ϊ��");
	for (i = 0; i < 4; i++)
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
	u8 usart_receive_message[20], i = 0, message_length = 0, mode = 0, receive_length = 0;;
	receive_length = Usart_Receivecheck();
	if (receive_length)
	{
		printf("debug");
		message_length = FSM(receive_length, usart_receive_message, &mode);
		switch (mode)
		{
		case 0XFF:                          //���Ĵ�  ���400
			MAXON_MOTOR[0] = (usart_receive_message[1] - 100) * 100 + (usart_receive_message[0] - 100) * 100;
			MAXON_MOTOR[1] = -((usart_receive_message[1] - 100) * 100 - (usart_receive_message[0] - 100) * 100);
			printf("��:%d",MAXON_MOTOR[0]);
			printf("��:%d",MAXON_MOTOR[1]);
			//report();
			break;

//		case 0XFE:                          
//			for(i=0;i<4;i++)
//			{
//				//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//				M3508[i].PID.Goal_Speed=(usart_receive_message[0]<<8)+usart_receive_message[1];
//			}
//			Report_Current();
//			break;

//		case 0XFD:												//������ת
//			for(i=0;i<message_length;i=i+2)
//			{
//				//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//				M3508[i/2].PID.Goal_Speed=((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//			}
//			Report_Current();
//			break;
//				
//		case 0XFC:												//������ת
//			for(i=0;i<message_length;i=i+2)
//			{
//				//printf("%d\r\n",(usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//				M3508[i/2].PID.Goal_Speed=-((usart_receive_message[i]<<8)+usart_receive_message[i+1]);
//			}
//			Report_Current();
//			break;

		case 0XFB:							//PID����
			if (message_length == 7)
			{
				for (i = 0; i < 7; i++)
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
			for (i = 0; i < message_length; i = i + 2)
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
				M3508[i / 2].Start_Flag = 1;
			}
			break;

		case 0XF9:
			RELAY = ~RELAY;
			break;

		default:
			break;
		}
		receive_length = 0;
	}
}


