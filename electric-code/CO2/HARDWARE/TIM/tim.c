#include <string.h>
#include <math.h>
#include "tim.h"
#include "can.h"
#include "adc.h"

/**************************************************************************
函数功能：定时器3初始化
入口参数：arr:重装载值 , psc: 预分频值
返回  值：无
**************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  			 
}


/**************************************************************************
函数功能：定时器3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
u16 adc = 0;

void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
//		static u16 queue[20] ={0};
//		s8 i;
//		s8 error = 0;
//		u16 front = 0, rear = 0;
//		u8 sendmessage[8] = {0};
//		float x;
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
		
		adc = Get_Adc_Average(2, 5);
		
		printf("%d\r\n",adc);
		
//		x = (float)adc;
//		memcpy(sendmessage, &x, 4);
//		CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
		
//		for (i = 18; i >= 0; i--)
//		{
//			queue[i + 1] = queue[i];
//			if(i < 9)
//				front += queue[i];
//			else
//				rear += queue[i];
//		}
//		queue[0] = adc;
//		front += adc;
//		front /= 10;
//		rear /= 10;
//		error = front - rear;
//		if(error < -5)
//		{
//			LED0 = 1;
//			x = 1;
//		}
//		else
//		{
//			LED0 = 0;
//			x = 0;
//		}

//		if (adc < 2500)
//		{
//			LED0 = 1;
//			x = 1;
//		}
//		else
//		{
//			LED0 = 0;
//			x = 0;
//		}

		
//		printf("CO2:%d\r\n",adc);
	}
}

void Message_Control()
{
	CanRxMsg RxMessage;
	float cmd;
	u8 sendmessage[8] = {0};
	float x = 0;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	switch(RxMessage.StdId)
	{
		// 接受主控消息
		case 0x09e0 >> 5 : 
		{
			memcpy(&cmd,RxMessage.Data+4,4);
			
			if(fabs(cmd -1.0f) < 0.1)
			{
				x = (float)adc;
				memcpy(sendmessage, &x, 4);
				CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
				printf("adc:  %d\r\n",adc);
				LED0 = ~LED0;
			}
			break;
		}
    
	}
}

void TIM5_Int_Init(u16 arr, u16 psc)		
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///使能TIM5时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//初始化TIM5
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //允许定时器5更新中断
	TIM_Cmd(TIM5, DISABLE); //使能定时器5
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			 
}

void TIM5_IRQHandler(void)   //TIM5中断
{
	static u8 i = 0;
	float cmd;
	u8 sendmessage[8] = {0};
	float x = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
		x = (float)adc;
		memcpy(sendmessage, &x, 4);
		CAN1_Send_Msg(sendmessage, 8, 0x09e0>>5);
		printf("adc:  %d\r\n",adc);
		LED0 = ~LED0;
		if(i++ >10)
			NVIC_SystemReset();
	}
}
