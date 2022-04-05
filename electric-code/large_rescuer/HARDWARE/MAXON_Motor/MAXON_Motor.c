#include <math.h>
#include "MAXON_Motor.h"

/**
  * @brief  MAXON�����ʼ������
  * @param  void
  * @retval void
  */

void Maxon_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  	//TIM2ʱ��ʹ��   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM2ʱ��ʹ��   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOCʱ��

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2); //GPIOB10����Ϊ��ʱ��2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0����Ϊ��ʱ��2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;  //��ʼ����������������2��3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1;  //��ʼ����������������2��3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//���ʹ��
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);
	
	//�����ת
	LEFT_Direction = 1;
	RIGHT_Direction = 1;

	//���������1��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;           //GPIOB10
	GPIO_Init(GPIOB, &GPIO_InitStructure);               //��ʼ��PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			//GPIOA0
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//��ʼ��PA0

	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ500������PWMƵ��Ϊ 1M/500=2Khz

	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = 500 - 1;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//��ʼ����ʱ��2
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//��ʼ����ʱ��2

	//��ʼ��PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(TIM2, ENABLE);//ARPEʹ�� 
	TIM_ARRPreloadConfig(TIM5, ENABLE);//ARPEʹ��

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM2
}

/**
  * @brief  MAXON�ٶ�����
  * @param  void
  * @retval void
  */

s16 MAXON_MOTOR[2]; //MAXON����ٶ�

void Maxon_Speed_Control(void)  // pwmֵ��10Hz--5kHz
{
	u16 pwm = 0;

	pwm = 50 + fabs(MAXON_MOTOR[0]) / 10;

	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[0] >= 0)
		LEFT_Direction = 1;
	else
		LEFT_Direction = 0;

	LEFT_Speed(pwm);

	pwm = 50 + fabs(MAXON_MOTOR[1]) / 10;
	if (pwm > 450)
		pwm = 450;
	else if (pwm < 50)
		pwm = 50;

	if (MAXON_MOTOR[1] >= 0)
		RIGHT_Direction = 1;
	else
		RIGHT_Direction = 0;

	RIGHT_Speed(pwm);
}

