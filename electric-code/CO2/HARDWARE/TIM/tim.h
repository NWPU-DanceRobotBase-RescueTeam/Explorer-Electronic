/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

extern u16 adc;

void TIM3_Int_Init(u16 arr,u16 psc);//��ʱ��3��ʼ��
void TIM5_Int_Init(u16 arr, u16 psc);

#endif

