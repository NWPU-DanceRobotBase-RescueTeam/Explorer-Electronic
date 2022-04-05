/**************************************************************************
���ߣ����δ�Ρ�����
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#include <string.h>
#include "tim.h"
#include "usart.h"
#include "OLED.h"
#include "key.h"
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

int limiting(int a,int b)
{
	if(a>=b) return b;
	else if(a<=-b)return -b;
	else return a;
}


s16 tadcxleft, tadcxright, tadcyright, tadcyleft;
u8 send_message[14] = { 0xff, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x0d, 0x0a };

void TIM5_IRQHandler(void)   //TIM3�ж�
{
	static s8 i;
	u8 j;
	float temp1, temp2;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־
		tadcxleft = limiting(-(Get_Adc_Average(ADC_Channel_13, 5) - 2048) / 200, 100);//��ҡ��x��      -10��10
		tadcyleft = limiting((Get_Adc_Average(ADC_Channel_12, 5) - 2048) / 200, 100);//��ҡ��y��
		tadcyright = limiting((Get_Adc_Average(ADC_Channel_5, 5) - 2048) / 200, 100);//��ҡ��y��
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
