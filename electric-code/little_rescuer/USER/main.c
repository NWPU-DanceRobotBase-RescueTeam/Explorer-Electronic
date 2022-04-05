#include <math.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "Motor_Control.h"
#include "fsm.h"
#include "debug.h"
#include "tim.h"
#include "can.h"
#include "sensor.h"
#include "adc.h"
#include "iwdg.h"
#include "spi.h"
#include "test.h"

//u8 PID_Flag = 0;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart1_init(115200);//初始化串口1波特率为115200
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);
	
	TIM4_Int_Init(0xffff,167);  //作为超声波的计时
	TIM7_Int_Init(100-1,5600-1);// 1/150s
	TIM2_PWM_Init(2000-1,840-1);// 20ms
	TIM3_Int_Init(15, 21000-1); // 3.75ms
	
	Wireless_serial_port_Init();
	LED_Init();
	
	SPI1_Init();
	Adc_Init();
	Sensor_Init();
		
	Control_Initialize();
	
	printf("start\r\n");
	
	TIM5_Int_Init(2000-1,8400-1);// 0.2s
	
//	EPD_4in2bc_test();
//	delay_ms(1000);
//	EPD_display();
	
	
	IWDG_Init(5,200);
	
	while(1) 
	{
		IWDG_Feed();
		debug();	
		if(refresh==1)
		{
			EPD_display();
			refresh=0;
		}
	}
}


