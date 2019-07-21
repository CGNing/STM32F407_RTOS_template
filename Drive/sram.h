/************************GUO NING*******************
作者：CGN
文件名：sram.h
创建日期：2018.6.4
维护人：
维护日期：
描述：
****************************************************/
#ifndef __SRAM_H
#define __SRAM_H

#include "stm32f4xx.h"

#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	

void FSMC_SRAM_Init(void);

#endif
