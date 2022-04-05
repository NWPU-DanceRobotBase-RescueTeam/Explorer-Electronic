/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
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

void TIM5_Int_Init(u16 arr,u16 psc);//定时器3初始化
void TIM3_Int_Init(u16 arr,u16 psc);
#endif

