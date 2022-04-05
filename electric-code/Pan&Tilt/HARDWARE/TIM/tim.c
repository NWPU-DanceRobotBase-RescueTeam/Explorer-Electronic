#include "tim.h"
#include "can.h"
#include "Angle.h"
#include "IMU.h"

/**************************************************************************
�������ܣ���ʱ��3��ʼ��
��ڲ�����arr:��װ��ֵ , psc: Ԥ��Ƶֵ
����  ֵ����
**************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx					 
}


/**************************************************************************
�������ܣ���ʱ��3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
extern float Angle[3];    

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	u8 Data[8],i;
	union ftc
	{
		float f;
		u8 t[4];
	} mid;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		LED0 = !LED0;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
		mid.f = -(Angle[ROLL]+2.0f);
		mid.f = mid.f / 180.0f * 3.1415926535f;
		for(i=0;i<4;i++)
			Data[i]=mid.t[i];
		mid.f = Angle[PITCH];
		mid.f = mid.f / 180.0f * 3.1415926535f;
		for(i=0;i<4;i++)
			Data[i+4]=mid.t[i];
		CAN1_Send_Msg(Data,8,0x0ae0>>5);
	}
}


