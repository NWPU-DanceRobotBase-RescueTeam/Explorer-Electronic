/**************************************************************************
作者：栋次大次
没有特别幸运，请先特别努力
**************************************************************************/
#ifndef _POT_H
#define _POT_H

#include "sys.h"

// 各副履带复位角度
#define Left_Front 198
#define Left_Rear 80
#define Right_Front 92
#define Right_Rear 210

void get_pot_adc_angle(u8 ID); // 电位器电压采集
void Subtrack_Rest(void);    // 副履带复位
void Angle_Position_Control(void);// 副履带位置控制

#endif
