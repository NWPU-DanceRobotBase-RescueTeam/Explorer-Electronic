/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include "tim.h"
#include "OLED.h"
#include "key.h"
#include "usart.h"
#include "led.h"
#include "adc.h"

/**
  * @brief  ��ʱ��5��ʼ��
  * @param  arr:��װ��ֵ
  *         psc: Ԥ��Ƶֵ
  * @retval void
  */

void TIM5_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  ///ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);//��ʼ��TIM3

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM5, ENABLE); //ʹ�ܶ�ʱ��3

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  ��ʱ��5�жϷ�����
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

u8 send_message[8] = { 0xff,0xff,0x02,0x0a,0x0a,0xfe,0x0d,0x0a };
s16 tadcxleft, tadcyright;

void TIM5_IRQHandler(void)   //TIM3�ж�
{
	u8 j;
	u8 static i;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־
		tadcxleft = limitings(-(Get_Adc_Average(ADC_Channel_13, 5) - 2048) / 200, 100);//��ҡ��x��      -10��10
//		adcyleft = limiting((Get_Adc_Average(ADC_Channel_12,5)-2060)/200,100);//��ҡ��y��
//		adcxright = limiting(-(Get_Adc_Average(ADC_Channel_4,5)-2050)/200,100);//��ҡ��x��
		tadcyright = limitings((Get_Adc_Average(ADC_Channel_5, 5) - 2048) / 200, 100);//��ҡ��y��

		Mode_Change();
		Show_Speed();
		if (adcxleft != send_message[3] || adcyright != send_message[4] || send_message[1] != 0xff)
		{
			send_message[3] = adcxleft;
			send_message[4] = adcyright;
			for (j = 0; j < 8; j++)
			{
				putchar(send_message[j]);
			}
		}
		if (send_message[1] != 0xff)
			send_message[1] = 0xff;
		i++;
		if (i > 10)
		{
			i = 0;
			LED0 = ~LED0;
		}
	}
}

//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//			 
//}
//void TIM3_IRQHandler(void)   //TIM3�ж�
//{
//	static u32 i;
//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//		{
//		  TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־
//			printf("%d  ",i++);
//		}
//	
//}
