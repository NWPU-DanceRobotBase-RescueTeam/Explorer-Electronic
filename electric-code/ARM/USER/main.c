#include <string.h>
#include <math.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "arm.h"
#include "can.h"
#include "tim.h"
#include "adc.h"
#include "pot.h"
#include "fsm.h"
#include "debug.h"
#include "Motor_Control.h"
#include "iwdg.h"
#include "exti.h"
#include "GIM8008.h"
/*
	GIM8008         ID1  一级臂    -1.57对应90°
	3508            ID2  二级臂    对应90°
	PB0  TIM3_CH3   ID3  底座选转  1500为中值
	PB1  TIM3_CH4	ID4  大轴向旋转 1500为直位置   
	PB3  TIM2_CH2	ID5	 摄像头自由度，1200同碳管竖直 2350向下极限 1000后仰极限（上下自由度）
	PB6  TIM4_CH1	ID6  爪子轴向旋转，1500正对着ID5
	PB7  TIM4_CH2	ID7  爪子 1450合 2150开
*/


void Message_Control()
{
	static s16 base_receive = 0, pwm4_receive = 0, pwm5_receive = 0, pwm6_receive = 0, pwm7_receive = 0;
//	static float gim_receive = 0;
	s16 int_temp = 0;
	float float_temp = 0;
	float radian = 0, radian2 = 0;
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	switch (RxMessage.StdId)
		{
		case 0x2f:  //0x05e0>>5
			memcpy(&radian, RxMessage.Data, 4);		      // -2.356 - 0.0 - 2.356
			printf("3:%.2f\r\n", radian);
			radian = radian + 2.243f;
			int_temp = (int)(radian * SERVO_RAD1 + 500);
			if (int_temp <= 500)
				int_temp = 500;
			else if (int_temp >= 2500)
				int_temp = 2500;
			if (int_temp != base_receive)
			{
//				Motor[0].Start_Flag = 0;
				base_receive = int_temp;
				servo_set(ID3, base_receive);
			}
			printf("3:%.2d\r\n", base_receive);
			break;

		case 0x37:  //0x06e0>>5					
			memcpy(&radian, RxMessage.Data, 4);
			float_temp = -radian;             				// 0 - 3.14
			if (float_temp <= -4)
				float_temp = -4;
			else if (float_temp >= 0)
				float_temp = 0;
			if (fabs(float_temp - first_arm_goal_position) < 0.5)
			{
				if (float_temp != first_arm_goal_position)
				{
					first_arm_goal_position = float_temp;
					Send_8008_Control((first_arm_goal_position + 0.25f) * 0.8f, 0, GIM_Kp, GIM_Kd, 0);
				}
				printf("1:%.2f\r\n", first_arm_goal_position);
			}

			memcpy(&radian2, &RxMessage.Data[4], 4);              // 0 - -2.16
			radian2 = -radian2;
			//printf("2:%.2f\r\n", radian2);
			float_temp = (float)(radian2 * MOTOR_RAD);
			if (float_temp >= 270)
				float_temp = 270.0f;
			else if (float_temp <= 0)
				float_temp = 0;
			if (float_temp != second_arm_goal_postion)
			{
				second_arm_goal_postion = float_temp;
				Motor[0].Start_Flag = 1;
	//			max_speed = 85;
	//			Motor[0].PID.Error_Sum = 0;
			}
			printf("2:%.2f\r\n", second_arm_goal_postion);
			
			break;

		case 0x3f:  //0x07e0>>5
			memcpy(&radian, RxMessage.Data, 4);                 // -1.57 - 0 - 1.57
			radian = -radian + 1.57f;
			int_temp = (int)(radian * SERVO_RAD + 500);
			if (int_temp <= 500)
				int_temp = 500;
			else if (int_temp >= 2500)
				int_temp = 2500;
			if (int_temp != pwm4_receive)
			{
//				Motor[0].Start_Flag = 0;
				pwm4_receive = int_temp;
				servo_set(ID4, pwm4_receive);
			}
			printf("4:%d\r\n", pwm4_receive);

			memcpy(&radian, &RxMessage.Data[4], 4);             // -1.81 - 0 - 0.3  
			radian = -radian + 1.09f;
			int_temp = (int)(radian * SERVO_RAD + 500);
			if (int_temp <= 1000)
				int_temp = 1000;
			else if (int_temp >= 2350)
				int_temp = 2350;
			if (int_temp != pwm5_receive)
			{
//				Motor[0].Start_Flag = 0;
				pwm5_receive = int_temp;
				servo_set(ID5, pwm5_receive);
			}
			printf("5:%d\r\n", pwm5_receive);
			break;

		case 0x47:  //0x08e0>>5
			memcpy(&radian, RxMessage.Data, 4);               // 0 - 1
			radian = radian + 1.53f;
			int_temp = (int)(radian * SERVO_RAD + 500);
			if (int_temp <= 1450)
				int_temp = 1450;
			else if (int_temp >= 2150)
				int_temp = 2150;
			if (int_temp != pwm7_receive)
			{
//				Motor[0].Start_Flag = 0;
				pwm7_receive = int_temp;
				servo_set(ID7, pwm7_receive);
			}
			printf("7:%d\r\n", pwm7_receive);

			memcpy(&radian, &RxMessage.Data[4], 4);           //  -1.57 - 0 - 1.57
			radian = -radian + 1.57f;
			int_temp = (int)(radian * SERVO_RAD + 500);
			if (int_temp <= 500)
				int_temp = 500;
			else if (int_temp >= 2500)
				int_temp = 2500;
			if (int_temp != pwm6_receive)
			{
//				Motor[0].Start_Flag = 0;
				pwm6_receive = int_temp;
				servo_set(ID6, pwm6_receive);
			}
			printf("6:%d\r\n", pwm6_receive);
			break;

	//	case 0x5f:	//0x0be0>>5
	//		memcpy(&float_temp, &RxMessage.Data[4], 4);
	//		if (fabs(float_temp - 1) <= 0.01)
	//		{
	//			EXTI_GenerateSWInterrupt(EXTI_Line1);
	//		}
	//		break;

		default:
			break;
		}
		LED0 = ~LED0;
}


int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200 
	EXTIX_Init();
	Adc_Init();
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq,CAN_BS1_7tq, 3, CAN_Mode_Normal);//CAN1初始化1Mbps 	
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq,CAN_BS1_7tq, 3, CAN_Mode_Normal);//CAN2初始化1Mbps 	
	
	Wireless_serial_port_Init();
	Control_Initialize();//电机结构体初始化
	servo_init();
	MotorControl_Start();
	LED_Init();
	
	Arm_Rest();
	
	TIM5_Int_Init(20-1, 42000-1); // 10ms中断一次		*
	IWDG_Init(6, 200);
	
	printf("Init\r\n");
	while(1) 
	{
		Message_Control();
		get_pot_adc_angle();
		IWDG_Feed();
		debug();
	}
}


