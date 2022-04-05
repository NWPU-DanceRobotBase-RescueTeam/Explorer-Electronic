/*
 * key.h
 *
 *  Created on: 2021年7月18日
 *      Author: 神祖
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#include "main.h"

#define key1 HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define key3 HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)
#define key4 HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)
#define key5 HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin)//左按钮
#define key6 HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin)//右按钮
#define key7 HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin)//左摇杆按键
#define key8 HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin)//左摇杆按键

void Mode_Change(void);
void Key_Scan(void);

extern int16_t adcxleft, adcyright;
extern uint8_t mode;

#endif /* INC_KEY_H_ */
