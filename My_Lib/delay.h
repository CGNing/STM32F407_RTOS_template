/************************GUO NING*******************
���ߣ�CGN
�ļ�����delay.h
�������ڣ�2018.5.5
ά���ˣ�
ά�����ڣ�
������
****************************************************/
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void delay_conf(u32 SYSCLK);
void delay_ms(u32 t);//0.0Xus�����,���ڼ�����������ж�����ʱ����ȡ��ԭ����ʱ
void delay_us(u32 t);

#endif
