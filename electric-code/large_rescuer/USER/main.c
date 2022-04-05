#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "fsm.h"
#include "Motor_Control.h"
#include "tim.h"
#include "debug.h"
#include "adc.h"
#include "pot.h"
#include "can.h"
#include "MAXON_Motor.h"
#include "iwdg.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);//初始化串口波特率为115200 
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);//CAN1初始化1Mbps 	
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);//CAN2初始化1Mbps 		
	LED_Init();
	Adc_Init();
	RELAY_Init();                
//	TIM4_Int_Init(700, 42000-1);   //暴走保险，硬件调试时请屏蔽
	TIM4_Int_Init(30, 42000 - 1);  // 10ms
	IWDG_Init(7, 800);

	Wireless_serial_port_Init();
	Maxon_Motor_Init();
	Control_Initialize();//电机结构体初始化

	Subtrack_Rest();//副履带复位
	printf("Hello");
	while (1)
	{
		IWDG_Feed();
		
		debug();
		Angle_Position_Control();
		LED0 = ~LED0;
	}
}






