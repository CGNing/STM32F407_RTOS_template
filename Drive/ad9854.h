/************************GUO NING*******************
作者：CGN
文件名：ad9854.h
创建日期：2017.8.13
维护人：
维护日期：
描述：
****************************************************/
#ifndef __AD9854_H
#define __AD9854_H

#include "stm32f10x.h"
#include "userdef.h"
#include "delay.h"

#define OSC 20	//晶振频率
#define CLK_SET 0X0A	//倍频数 

#define ADDR_POR1 	0X00	//Phase Offset Tuning Word Register 1			2
#define ADDR_POR2 	0X01	//Phase Offset Tuning Word Register 2			2
#define	ADDR_FREQ1	0X02	//Frequency Tuning Word 1									6
#define	ADDR_FREQ2	0X03	//Frequency Tuning Word 2									6
#define	ADDR_DFREQ	0X04	//Delta frequency register								6
#define	ADDR_UDCLKR	0X05	//Update clock rate register							4
#define	ADDR_RRCLK	0X06	//Ramp rate clock register								3
#define	ADDR_CR			0X07	//Control register												4
#define	ADDR_IPDMR	0X08	//I path digital multiplier register			2
#define	ADDR_QPDMR	0X09	//Q path digital multiplier register			2
#define	ADDR_SHAPED	0X0A	//Shaped on/off keying ramp rate register	1
#define	ADDR_QDACR	0X0B	//Q DAC register													2

#define MASTER_RESET_H GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define MASTER_RESET_L GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define RESET_H	GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define RESET_L	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define UDCLK_H	GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define UDCLK_L	GPIO_ResetBits(GPIOC,GPIO_Pin_2)

#define AD9854_CS_H GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define AD9854_CS_L GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define AD9854_CLK_H GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define AD9854_CLK_L GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define AD9854_MOSI_H GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define AD9854_MOSI_L GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define AD9854_MISO GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)

void AD9854_SPI2_Init(void);
void AD9854_Init(void);
void AD9854_WriteIntr(u8 cmd);
void AD9854_ReadIntr(u8 cmd);
u8 AD9854_ReadWriteByte(u8 TxData);
void AD9854_FreqToFTW(unsigned long Freq,u8* buf);
void AD9854_SetSinFreq(u32 Freq,u16 Shape);//设置正弦频率 MHz


#endif
