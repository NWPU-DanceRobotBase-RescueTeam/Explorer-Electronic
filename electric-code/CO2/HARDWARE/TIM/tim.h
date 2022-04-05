/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

extern u16 adc;

void TIM3_Int_Init(u16 arr,u16 psc);//定时器3初始化
void TIM5_Int_Init(u16 arr, u16 psc);

#endif

