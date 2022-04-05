#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "delay.h"

void TIM3_Int_Init(u16 arr,u16 asc);
void TIM3_IRQHandler(void);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM5_IRQHandler(void);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM4_IRQHandler(void) ;

#endif
