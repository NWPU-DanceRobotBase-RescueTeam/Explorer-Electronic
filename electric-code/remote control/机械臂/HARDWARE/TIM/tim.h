/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/

#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

extern s16 tadcxleft, tadcxright, tadcyright, tadcyleft;
extern u8 send_message[14];

void TIM5_Int_Init(u16 arr, u16 psc);//��ʱ��3��ʼ��
void TIM3_Int_Init(u16 arr, u16 psc);

#endif

