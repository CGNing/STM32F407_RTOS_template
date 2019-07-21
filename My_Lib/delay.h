/************************GUO NING*******************
作者：CGN
文件名：delay.h
创建日期：2018.5.5
维护人：
维护日期：
描述：
****************************************************/
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void delay_conf(u32 SYSCLK);
void delay_ms(u32 t);//0.0Xus的误差,但期间如果有其他中断有延时会有取消原有延时
void delay_us(u32 t);

#endif
