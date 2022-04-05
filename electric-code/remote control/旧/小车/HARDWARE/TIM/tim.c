/**************************************************************************
作者：栋次大次、瑞瑞
没有特别幸运，请先特别努力
**************************************************************************/

#include "tim.h"
#include "OLED.h"
#include "adc.h"
/**************************************************************************
函数功能：定时器3初始化
入口参数：arr:重装载值 , psc: 预分频值
返回  值：无
**************************************************************************/

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			 
}


/**************************************************************************
函数功能：定时器3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/

int limiting(int a,int b)
{
	if(a>=b) return b;
	else if(a<=-b)return -b;
	else return a;
}
u8 tadcxleft,tadcyright;
void TIM5_IRQHandler(void)   //TIM3中断
{
	static u8 i;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志
		tadcxleft = limiting(-(Get_Adc_Average(ADC_Channel_13,5)-2048)/200,100);//左摇杆x轴      -10到10
//	adcyleft = limiting((Get_Adc_Average(ADC_Channel_12,5)-2060)/200,100);//左摇杆y轴
//	adcxright = limiting(-(Get_Adc_Average(ADC_Channel_4,5)-2050)/200,100);//右摇杆x轴
		tadcyright = limiting((Get_Adc_Average(ADC_Channel_5,5)-2048)/200,100);//右摇杆y轴
			if(i++>10)
			{
				i =0;
				if(LED0 == 1)
				{
					LED1 = 1;
					LED0 = 0;
				}
				else if(LED1 == 1)
				{
					LED2 = 1;
					LED1 = 0;
				}
				else if(LED2 == 1)
				{
					LED3 = 1;
					LED2 = 0;
				}
				else if(LED3 == 1)
				{
					LED0 = 1;
					LED3 = 0;
				}
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
