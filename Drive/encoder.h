/************************GUO NING*******************
作者：CGN
文件名：encoder.h
创建日期：2017.9.17
维护人：
维护日期：
描述：
****************************************************/
#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f4xx.h"
 
 
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);
int Read_Encoder(u8 TIMX);

//void TIM2_IRQHandler(void);
//void TIM3_IRQHandler(void);
//void TIM4_IRQHandler(void);
//void TIM5_IRQHandler(void);
#endif
