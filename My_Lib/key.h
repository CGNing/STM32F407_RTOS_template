/************************GUO NING*******************
作者：CGN
文件名：key.h
创建日期：2018.3.11
维护人：
维护日期：
描述：
****************************************************/
#ifndef __KEY_H
#define __KEY_H

#include "stm32f4XX.h"

#define KEY0      (!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5))
#define KEY1      (!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6))
#define KEY2      (!GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7))


void KEY_Init(void);
u8 KEY_read(void);

#endif
