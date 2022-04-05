#include "delay.h"
#include "usart.h"
#include <string.h>
#include <math.h>
#include "led.h"
#include "can.h"
#include "tim.h"
#include "iwdg.h"
#include "adc.h"

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart1_init(115200);//��ʼ������1������Ϊ115200
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 3, CAN_Mode_Normal);
//	IIC_Init();
	
//	CJ811_Init();
//	delay_ms(10);
	Wireless_serial_port_Init();
	LED_Init();
	Adc_Init();
	
	TIM3_Int_Init(1000-1, 8400-1);	// 100ms	
	TIM5_Int_Init(5000-1, 8400-1);  // 500ms
	IWDG_Init(7, 500);
	
	printf("init\r\n");
	
	while(1)
	{
		IWDG_Feed();
	}
}


