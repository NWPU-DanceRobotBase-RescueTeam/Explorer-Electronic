#include "exti.h"
#include "delay.h"
#include "arm.h"
#include "GIM8008.h"
#include "pot.h"
#include "Motor_Control.h"

/**
  * @brief  ������ת����
  * @param  void
  * @retval void
  */

const float closing = 0.0;
const float opening = 0.6;
const float clockwise = 1.57;

void MechanicalClaw_Quickcommand(void)
{
	float radian = 0;
	s16 pwm_receive = 0;
	radian = closing + 1.43f;
	pwm_receive = (int)(radian * SERVO_RAD + 500);
	servo_set(ID7, pwm_receive);
	radian = 0 - clockwise + 1.57f;
	pwm_receive = (int)(radian * SERVO_RAD + 500);
	servo_set(ID6, pwm_receive);
	delay_ms(200);

	radian = opening + 1.43f;
	pwm_receive = (int)(radian * SERVO_RAD + 500);
	servo_set(ID7, pwm_receive);
	radian = clockwise + 1.57f;
	pwm_receive = (int)(radian * SERVO_RAD + 500);
	servo_set(ID6, pwm_receive);
	delay_ms(200);
}

// ֹͣ
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
	MotorControl_Start();
	Motor[0].Start_Flag = 0;
}	

// ������ת
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE1�ϵ��жϱ�־λ 
	MechanicalClaw_Quickcommand();
}	

/**
  * @brief  �ⲿ�жϳ�ʼ������
  * @param  void
  * @retval void
  */

void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
    /* ����EXTI_Line0,Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;//LINE0,LINE1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�0           ������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�3           ������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	   
}












