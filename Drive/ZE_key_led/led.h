#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

#define LED1_ON GPIO_ResetBits(GPIOF,GPIO_Pin_9);
#define LED1_OFF GPIO_SetBits(GPIOF,GPIO_Pin_9);
#define LED2_ON GPIO_ResetBits(GPIOF,GPIO_Pin_10);
#define LED2_OFF GPIO_SetBits(GPIOF,GPIO_Pin_10);

void LED_Init(void);

//const u8 EightDigital[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

#endif
