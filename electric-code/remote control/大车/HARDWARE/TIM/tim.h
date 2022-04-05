/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern s16 tadcxleft, tadcyright, tadcyleft;
extern u8 send_message[8], send_message2[14], fuwei;

void TIM5_Int_Init(u16 arr, u16 psc);//定时器3初始化
void TIM3_Int_Init(u16 arr, u16 psc);

#endif

