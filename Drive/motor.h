/************************GUO NING*******************
���ߣ�CGN
�ļ�����motor.h
�������ڣ�2018.5.22
ά���ˣ�
ά�����ڣ�
������
****************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"

void motor_Init(void);
void move(int L,int R);//0~1000 ����PWMƵ�ʲ��ܳ���60KHz,ռ�ձȲ��ܳ���98%

#endif
