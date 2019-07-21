/************************GUO NING*******************
作者：CGN
文件名：i2c_soft.h
创建日期：2017.9.3
维护人：
维护日期：
描述：
****************************************************/
#ifndef __I2C_SOFT_H
#define __I2C_SOFT_H

#include "stm32f4xx.h"

#define I2C_SCL_RCC       RCC_AHB1Periph_GPIOB
#define I2C_SCL_GPIO      GPIOB
#define I2C_SCL_PIN       GPIO_Pin_12
#define I2C_SDA_RCC       RCC_AHB1Periph_GPIOB
#define I2C_SDA_GPIO      GPIOB
#define I2C_SDA_PIN       GPIO_Pin_13

#define I2C_SDA_IN        {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=8<<20;}
#define I2C_SDA_OUT       {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=3<<20;}

#define I2C_SCL_H         I2C_SCL_GPIO->BSRRH = I2C_SCL_PIN
#define I2C_SCL_L         I2C_SCL_GPIO->BSRRL  = I2C_SCL_PIN
#define I2C_SDA_H         I2C_SDA_GPIO->BSRRH = I2C_SDA_PIN
#define I2C_SDA_L         I2C_SDA_GPIO->BSRRL  = I2C_SDA_PIN
#define I2C_SCL_read      I2C_SCL_GPIO->IDR  & I2C_SCL_PIN
#define I2C_SDA_read      I2C_SDA_GPIO->IDR  & I2C_SDA_PIN

extern volatile u8 I2C_FastMode;

void I2c_Soft_Init(void);
void I2c_Soft_SendByte(u8 SendByte);
u8 I2c_Soft_ReadByte(u8);

u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
u8 IIC_Read_1Byte(u8 SlaveAddress,u8 REG_Address,u8 *REG_data);
u8 IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);
u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf);

#endif
