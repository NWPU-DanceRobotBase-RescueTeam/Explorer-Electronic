/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/

#include <string.h>
#include "tim.h"
#include "usart.h"
#include "OLED.h"
#include "key.h"
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

int limiting(int a,int b)
{
	if(a>=b) return b;
	else if(a<=-b)return -b;
	else return a;
}


s16 tadcxleft, tadcxright, tadcyright, tadcyleft;
u8 send_message[14] = { 0xff, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0d, 0x0a };

void TIM5_IRQHandler(void)   //TIM3中断
{
	static s8 i;
	u8 j;
	float temp1, temp2;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志
		tadcxleft = limiting(-(Get_Adc_Average(ADC_Channel_13, 5) - 2048) / 200, 100);//左摇杆x轴      -10到10
		tadcyleft = limiting((Get_Adc_Average(ADC_Channel_12, 5) - 2048) / 200, 100);//左摇杆y轴
		tadcyright = limiting((Get_Adc_Average(ADC_Channel_5, 5) - 2048) / 200, 100);//右摇杆y轴
		tadcxright = limiting((Get_Adc_Average(ADC_Channel_4, 5) - 2048) / 200, 100);

		if (mode != lastmode)
			LCD_CLS();

		if (mode == 0 && mode != lastmode)
		{
			u8 stop_message[14] = { 0xff, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0d, 0x0a };
			for (j = 0; j < 14; j++)
			{
				putchar(stop_message[j]);
			}
		}

		Mode_Change();

		Show();

		if (mode == 1)
		{
			memcpy(&temp1, send_message + 3, 4);
			memcpy(&temp2, send_message + 7, 4);
			if (temp1 != paw_open_close.Float || temp2 != paw_rotation.Float)
			{
				send_message[1] = 0x38;
				send_message[3] = paw_open_close.Binary[0]; send_message[4] = paw_open_close.Binary[1]; send_message[5] = paw_open_close.Binary[2]; send_message[6] = paw_open_close.Binary[3];
				send_message[7] = paw_rotation.Binary[0]; send_message[8] = paw_rotation.Binary[1]; send_message[9] = paw_rotation.Binary[2]; send_message[10] = paw_rotation.Binary[3];
				for (j = 0; j < 14; j++)
				{
					putchar(send_message[j]);
				}
			}
		}
		if (mode == 2)
		{
			memcpy(&temp1, send_message + 3, 4);
			memcpy(&temp2, send_message + 7, 4);
			if (temp1 != small_rotation.Float || temp2 != small_oscillation.Float)
			{
				send_message[1] = 0x27;
				send_message[3] = small_rotation.Binary[0]; send_message[4] = small_rotation.Binary[1]; send_message[5] = small_rotation.Binary[2]; send_message[6] = small_rotation.Binary[3];
				send_message[7] = small_oscillation.Binary[0]; send_message[8] = small_oscillation.Binary[1]; send_message[9] = small_oscillation.Binary[2]; send_message[10] = small_oscillation.Binary[3];
				for (j = 0; j < 14; j++)
				{
					putchar(send_message[j]);
				}
			}
		}
		if (mode == 3)
		{
			memcpy(&temp1, send_message + 3, 4);
			memcpy(&temp2, send_message + 7, 4);
			if (temp1 != first_arm.Float || temp2 != second_arm.Float)
			{
				send_message[1] = 0x16;
				send_message[3] = first_arm.Binary[0]; send_message[4] = first_arm.Binary[1]; send_message[5] = first_arm.Binary[2]; send_message[6] = first_arm.Binary[3];
				send_message[7] = second_arm.Binary[0]; send_message[8] = second_arm.Binary[1]; send_message[9] = second_arm.Binary[2]; send_message[10] = second_arm.Binary[3];
				for (j = 0; j < 14; j++)
				{
					putchar(send_message[j]);
				}
			}
		}
		if (mode == 4)
		{
			memcpy(&temp1, send_message + 3, 4);
//			memcpy(&temp2,send_message+7,4);
			if (temp1 != base.Float)
			{
				send_message[1] = 0x05;
				send_message[3] = base.Binary[0]; send_message[4] = base.Binary[1]; send_message[5] = base.Binary[2]; send_message[6] = base.Binary[3];
//				send_message[7] = paw_rotation.Binary[0];send_message[8] = paw_rotation.Binary[1];send_message[9] = paw_rotation.Binary[2];send_message[10] = paw_rotation.Binary[3];
				for (j = 0; j < 14; j++)
				{
					putchar(send_message[j]);
				}
			}
		}

		if (spin == 1)
		{
			spin = 0;
			send_message[1] = 0x49;
			for (j = 3; j <= 10; j++)
			{
				send_message[j] = 0;
			}
			for (j = 0; j < 14; j++)
			{
				putchar(send_message[j]);
			}
			Show_Spin();
		}

		if (reset == 1)
		{
			reset = 0;
			send_message[1] = 0x5a;
			for (j = 3; j <= 10; j++)
			{
				send_message[j] = 0;
			}
			for (j = 0; j < 14; j++)
			{
				putchar(send_message[j]);
			}
			Show_Reset();
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
