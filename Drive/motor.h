/************************GUO NING*******************
作者：CGN
文件名：motor.h
创建日期：2018.5.22
维护人：
维护日期：
描述：
****************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

void motor_Init(void);
void move(int L,int R);//0~1000 驱动PWM频率不能超过60KHz,占空比不能超过98%

#endif
