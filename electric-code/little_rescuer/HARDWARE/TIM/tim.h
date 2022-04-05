/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

//extern u8 PID_Flag;
//extern u32 PID_coefficient[5];
void TIM3_Int_Init(u16 arr,u16 psc);//定时器3初始化
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

#endif

