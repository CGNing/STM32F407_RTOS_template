/************************GUO NING*******************
作者：CGN
文件名：ads5500.h
创建日期：2017.7.15
维护人：
维护日期：
描述：
****************************************************/
#ifndef __CDS5500_H
#define __CDS5500_H

#include "stm32f10x.h"
#include "userdef.h"
#include "delay.h"

#define BOTHWAY	//双向通信

#define CMD_PING			0X01		//查询								0
#define CMD_READ			0X02		//数字舵机读命令			2
#define CMD_WRITE			0X03		//数字舵机写命令			>2
#define CMD_REGWRITE	0X04		//异步写							>2
#define CMD_ACTION		0X05		//执行异步写					0
#define CMD_RESET			0X06		//复位								0
#define CMD_SWRITE		0X83		//数字舵机同步写命令	0


#define CDS_SEVMODE			0			//舵机模式
#define CDS_MOTOMODE		1			//电机模式

#ifdef BOTHWAY

#define UART3_TXEN (GPIOC->ODR |= GPIO_Pin_5)	//使能
#define UART3_TXDS (GPIOC->ODR &= ~GPIO_Pin_5)	//失能
#define UART3_RXEN (GPIOC->ODR |= GPIO_Pin_4)
#define UART3_RXDS (GPIOC->ODR &= ~GPIO_Pin_4)

#else

#define UART3_TXEN
#define UART3_TXDS
#define UART3_RXEN
#define UART3_RXDS

#endif

void CDS_conf(void);	//初始化
void CDS_cmd(u8 id, u8 len, u8 cmd, u8* pData);	//发送命令
void CDS_SetMode(u8 id,u8 mode);	//设置模式
void CDS_SetAngle(u8 id,u16 angle,u16 speed);	//设置舵机角度
//void CDS_SetAngleTimes(u8 id, u16 angle, s16 speed,u8 times);	//设置数字舵机（舵机模式下）角度和速度
void CDS_SetSpeed(u8 id,s16 speed);	//设置电机转速
//void CDS_Set4MotoSpeed(u8 id[4], s16 speed[4]);	//同时设置4路数字舵机的速度（电机模式下）



#endif
