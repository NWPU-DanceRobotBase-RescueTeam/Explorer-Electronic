/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/

#include "tim.h"
#include "OLED.h"
#include "key.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

/**
  * @brief  定时器5初始化
  * @param  arr:重装载值
  *         psc: 预分频值
  * @retval void
  */

void TIM5_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//初始化TIM3

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5, ENABLE); //使能定时器3

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  定时器5中断服务函数
  * @param  void
  * @retval void
  */

s16 limitings(s16 a, s16 b)
{
    if (a >= b) 
		return b;
    else if (a <= -b)
		return -b;
    else 
		return a;
}


s16 tadcxleft, tadcyright, tadcyleft;
u8 send_message[8] = { 0xff, 0xff, 0x02, 0x0a, 0x0a, 0xfe, 0x0d, 0x0a };
u8 send_message2[14] = { 0xff, 0xfa, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0d, 0x0a };


void TIM5_IRQHandler(void)   //TIM3中断
{
	static s8 i, j;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志
		tadcxleft = limitings(-(Get_Adc_Average(ADC_Channel_13, 5) - 2048) / 200, 100);//左摇杆x轴      -10到10
		tadcyleft = limitings((Get_Adc_Average(ADC_Channel_12, 5) - 2048) / 200, 100);//左摇杆y轴
		tadcyright = limitings((Get_Adc_Average(ADC_Channel_5, 5) - 2048) / 200, 100);//右摇杆y轴

		Mode_Change();
		
		if (mode != lastmode)
			LCD_CLS();
		if (mode == 0 || mode == 1 || mode == 2 || mode == 3)
			Show_Speed();
		else if (mode == 4 || mode == 5)
			Show_Angel();

		if (mode == 0 || mode == 1 || mode == 2 || mode == 3)
		{
			if (adcxleft != send_message[3] || adcyright != send_message[4])
			{
				send_message[3] = adcxleft;
				send_message[4] = adcyright;
				for (j = 0; j < 8; j++)
				{
					putchar(send_message[j]);
				}
			}
		}
//		else if(mode==4||mode==5)
//		{
		if (fulvdai[0] != ((send_message2[3] << 8) + send_message2[4]) || fulvdai[1] != ((send_message2[5] << 8) + send_message2[6]) || 
			fulvdai[2] != ((send_message2[7] << 8) + send_message2[8]) || fulvdai[3] != ((send_message2[9] << 8) + send_message2[10]))
		{
			send_message2[3] = (u8)(fulvdai[0] >> 8);
			send_message2[4] = (u8)fulvdai[0];
			send_message2[5] = (u8)(fulvdai[1] >> 8);
			send_message2[6] = (u8)fulvdai[1];
			send_message2[7] = (u8)(fulvdai[2] >> 8);
			send_message2[8] = (u8)fulvdai[2];
			send_message2[9] = (u8)(fulvdai[3] >> 8);
			send_message2[10] = (u8)fulvdai[3];
			for (j = 0; j < 14; j++)
			{
				putchar(send_message2[j]);
			}
//			}7
		}
		if (i++ > 10)
		{
			LED0 = ~LED0;
			i = 0;
		}
	}

}

//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
//	TIM_Cmd(TIM3,ENABLE); //使能定时器3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//			 
//}
//void TIM3_IRQHandler(void)   //TIM3中断
//{
//	static u32 i;
//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//		{
//		  TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志
//			printf("%d  ",i++);
//		}
//	
//}
