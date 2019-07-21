/************************GUO NING*******************
作者：CGN
文件名：servo.h
创建日期：2018.5.5
维护人：
维护日期：
描述：
****************************************************/
#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f4xx.h"
#include "delay.h"

void servo_Init(void);
void servo_angle(u8 id,u8 angle);
void servo_time(u8 id,u16 time);
void servo_test(u8 id);


#endif
