#ifndef _IIC_H
#define _IIC_H

#include "sys.h"

//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PB9输出模式
//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define SDA_read      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

void I2C_Stop(void);

u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data);
u8 Single_MWriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length);
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
#endif
