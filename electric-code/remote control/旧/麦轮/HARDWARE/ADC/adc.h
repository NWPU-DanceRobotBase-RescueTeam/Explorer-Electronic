/**************************************************************************
���ߣ����δ��
û���ر����ˣ������ر�Ŭ��
**************************************************************************/
#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Motor_Control.h"
														   
void  Adc_Init(void);//ADC��ʼ��
u16 Get_Adc_Average(u8 ch,u8 times); //��ȡ���ADC�ɼ���ƽ��ֵ
u16 Adc_Check_Rest(void);

#endif

