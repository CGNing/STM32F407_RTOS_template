/************************GUO NING*******************
作者：CGN
文件名：usart.h
创建日期：2019.7.20
维护人：
维护日期：
描述：
****************************************************/

#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"

#define UART_TX GPIO_Pin_9
#define UART_RX GPIO_Pin_10
#define USART_get() void USART1_IRQHandler(void)

void USART1_Init(u32 baud);
void USART2_Init(u32 baud);

void USART_Send_Byte(USART_TypeDef* USARTx,u8 data);
void USART_Send_nByte(USART_TypeDef* USARTx,u8* data);
void USART_Send_Str(USART_TypeDef* USARTx,u8* data);
void USART_Send_Num(USART_TypeDef* USARTx,int Num);

#endif
