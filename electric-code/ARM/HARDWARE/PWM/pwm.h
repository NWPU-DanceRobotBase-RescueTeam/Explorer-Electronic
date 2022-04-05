#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"

void TIM4_PWM_Init(u32 arr, u32 psc);//定时器TIM4,TIM3,TIM2 设置为PWM模式
void TIM3_PWM_Init(u32 arr, u32 psc);
void TIM2_PWM_Init(u32 arr, u32 psc);

#endif

