/************************GUO NING*******************
作者：CGN
文件名：ov2640.h
创建日期：2018.6.4
维护人：
维护日期：
描述：
****************************************************/
#ifndef __OV2640_DCMI_H
#define __OV2640_DCMI_H

#include "stm32f4xx.h"

void dcmi_init(void);
void dcmi_dma_init(u32 DMA_Memory0BaseAddr,u32 DMA_MemoryDataSize);

#endif
