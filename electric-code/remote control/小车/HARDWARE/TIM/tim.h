/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern s16 tadcxleft, tadcyright;

extern u8 send_message[8];

void TIM5_Int_Init(u16 arr, u16 psc);//��ʱ��3��ʼ��
void TIM3_Int_Init(u16 arr, u16 psc);

#endif

