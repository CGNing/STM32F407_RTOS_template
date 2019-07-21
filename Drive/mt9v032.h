/************************GUO NING*******************
作者：CGN
文件名：mt9v032.h
创建日期：2018.3.14
维护人：
维护日期：
描述：
****************************************************/
#ifndef __MT9V032_H
#define __MT9V032_H

#include "stm32f4xx.h"
#include "mtiic.h"
#include "usart.h"
#include "delay.h"

#define PRINTF

#define MT9V032_WIDTH     	240
#define MT9V032_HEIGHT    	320

extern u8 pic_data[MT9V032_WIDTH*MT9V032_HEIGHT];

s8 MT_IIC_WR_REG(u8 reg,u16 data);
s8 MT_IIC_RD_REG(u8 reg,u16* data);	
s8 mt9v032_Init(void);
void dcmi_init(void);
void dcmi_dma_init(void);
void mt9v032_cmd(FunctionalState NewState);
u16 mt9v032_get_version(void);
s8 mt9v032_set_fps(u8 fps);
#endif
