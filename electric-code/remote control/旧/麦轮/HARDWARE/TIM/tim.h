/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
#include "led.h"
#include "adc.h"
#include "pot.h"

extern s8 tadcxleft,tadcyright,tadcyleft,fuwei;

void TIM5_Int_Init(u16 arr,u16 psc);//��ʱ��3��ʼ��
void TIM3_Int_Init(u16 arr,u16 psc);
#endif

