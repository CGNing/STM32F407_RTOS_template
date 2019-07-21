/************************GUO NING*******************
作者：CGN
文件名：esp8266.h
创建日期：2018.6.8
维护人：
维护日期：
描述：
****************************************************/
#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f4xx.h"

#define ESP8266_USART USART3
#define PRINTF

s8 esp8266_init(void);
s8 esp8266_cmd(char* data);
s8 esp8266_printf_ip(void);
void esp8266_send_data(u8* data,u32 len);

#endif
