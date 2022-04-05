#include "exti.h"
#include "delay.h"
#include "arm.h"
#include "GIM8008.h"
#include "pot.h"
#include "Motor_Control.h"

/**
  * @brief  快速旋转动作
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

// 停止
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
	MotorControl_Start();
	Motor[0].Start_Flag = 0;
}	

// 快速旋转
void EXTI1_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位 
	MechanicalClaw_Quickcommand();
}	

/**
  * @brief  外部中断初始化程序
  * @param  void
  * @retval void
  */

void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
    /* 配置EXTI_Line0,Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;//LINE0,LINE1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0           最高优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3           最低优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	   
}












