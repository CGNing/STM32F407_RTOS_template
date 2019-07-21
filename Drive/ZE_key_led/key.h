#ifndef __KEY_H
#define __KEY_H

#include "stm32f4XX.h"

#define KEY0      !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KEY1      !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KEY_WK_UP  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

void KEY_Init(void);
u8 KEY_read(void);//up is 0001,down is 0010,left is 0100,right is 1000

#endif
