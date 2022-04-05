#ifndef __DEBUG__H
#define __DEBUG__H
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "fsm.h"
#include "Motor_Control.h"
#include "tim.h"
#include "adc.h"
#include "pot.h"

void debug(void);
void Report_Current(void);
void Report_ADC(void);



#endif
