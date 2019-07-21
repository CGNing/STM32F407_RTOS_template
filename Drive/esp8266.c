/************************GUO NING*******************
作者：CGN
文件名：esp8266.c
创建日期：2017.9.17
维护人：
维护日期：
描述：
管脚：
****************************************************/
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "led.h"

#include <stdio.h> 
#include <string.h>

struct _esp8266_ap{
	u8 ssid[16];
	u8 password[32];
	u8 ch;
	//0 OPEN 1 WEP 2 WPA_PSK 3 WPA2_PSK 4 WPA_WPA2_PSK
	u8 ecn;
};

struct _esp8266_cfg{
	u8 									atecho;
	u8 									mode;
	struct _esp8266_ap 	sap;									
};

struct _esp8266_ip{
	u8 	mux;
	u16	server;
};

struct _esp8266_cfg esp8266_cfg={
	.atecho=0,
	.mode=2,
	.sap={"runcar","0123456789",5,3}
};

struct _esp8266_ip esp8266_ip={
	.mux=1,
	.server=8000
};

u8 esp8266_rx_status=0;//0 receiving 1 received
u8 esp8266_rx_pos=0;
char esp8266_rx[128];

/**
	@brief		USART3中断函数
	@return		void
	@since		v1.0
*/
void USART3_IRQHandler(void){
	u8 buf;
	if(USART_GetITStatus(ESP8266_USART, USART_IT_RXNE) != RESET){
//		USART_ClearITPendingBit(ESP8266_USAR, USART_IT_RXNE);
		buf=USART_ReceiveData(ESP8266_USART);
		if(esp8266_rx_status==0){
			esp8266_rx[esp8266_rx_pos++]=buf;
		}
	}
	
	if(USART_GetITStatus(ESP8266_USART, USART_IT_IDLE) != RESET){
		USART_ClearITPendingBit(ESP8266_USART, USART_IT_IDLE);
		buf=USART_ReceiveData(ESP8266_USART);
		esp8266_rx[esp8266_rx_pos++]=buf;
		esp8266_rx[esp8266_rx_pos]='\0';
		esp8266_rx_pos=0;
		esp8266_rx_status=1;
	}
}

/**
	@brief      ESP8266初始化
	@return     void
	@since      v1.0
*/
s8 esp8266_init(void){
	char cmd[128];
	USART3_Init(115200);
	
//	esp8266_cmd("AT+RST");
//	delay_ms(2000);
//	LED2_ON;
	//test
	if(esp8266_cmd("AT"))
		return -1;
	//set
	sprintf(cmd,"ATE%d",esp8266_cfg.atecho);
	esp8266_cmd(cmd);
	sprintf(cmd,"AT+CWMODE=%d",esp8266_cfg.mode);
	esp8266_cmd(cmd);
	sprintf(cmd,"AT+CWSAP=\"%s\",\"%s\",%d,%d",esp8266_cfg.sap.ssid,esp8266_cfg.sap.password,esp8266_cfg.sap.ch,esp8266_cfg.sap.ecn);
	esp8266_cmd(cmd);
	//set tcp/ip udp
	sprintf(cmd,"AT+CIPMUX=%d",esp8266_ip.mux);
	esp8266_cmd(cmd);
	sprintf(cmd,"AT+CIPSERVER=%d,%d",esp8266_ip.server>0?1:0,esp8266_ip.server);
	esp8266_cmd(cmd);
	return 0;
}

/**
	@brief		ESP8266发送指令
	@param		char* 指令
	@return		void
	@since		v1.0
*/
s8 esp8266_cmd(char* data){
	u16 t;
	
	while((*data<='~')&&(*data>=' ')){
		while(USART_GetFlagStatus(ESP8266_USART,USART_FLAG_TXE)==RESET);
		USART_SendData(ESP8266_USART,*data);
		data++;
	}
	
	while(USART_GetFlagStatus(ESP8266_USART,USART_FLAG_TXE)==RESET);
		USART_SendData(ESP8266_USART,'\r');
	while(USART_GetFlagStatus(ESP8266_USART,USART_FLAG_TXE)==RESET);
		USART_SendData(ESP8266_USART,'\n');
	
	esp8266_rx_status=0;
	t=0;
	while(t++<100&&esp8266_rx_status!=1)
		delay_ms(1);
	
	if(t>=100||strstr(esp8266_rx,"OK")==NULL)
		return -1;
	return 0;
}

/**
	@brief		ESAP8266打印输出IP
	@return		void
	@since		v1.0
*/
s8 esp8266_printf_ip(void){
	u16 t=0;
	char temp_ip[14];
	char *temp_pstart,*temp_pend;
	
	esp8266_cmd("AT+CIFSR");
	
	t=0;
	while(t++<100&&esp8266_rx_status!=1)
		delay_ms(1);
	
	if(t>=100)
		return -1;
	
	temp_pstart=strstr(esp8266_rx,"APIP,");
	temp_pstart+=6;
	temp_pend=strchr(temp_pstart,'\"');
	strncpy(temp_ip,temp_pstart,temp_pend-temp_pstart);
	temp_ip[temp_pend-temp_pstart]='\0';
#ifdef PRINTF
	printf("ESP8266 ip is %s\n",temp_ip);
#endif
	return 0;
}

/**
	@brief		ESAP8266发送数据
	@param		u8* 数据数组指针
	@param		u32 数据长度
	@return		void
	@since		v1.0
*/
#define ESP8266_MAX_SIZE 2048
void esp8266_send_data(u8* data,u32 len){
	u16 loop=len/ESP8266_MAX_SIZE;
	u16 end=len%ESP8266_MAX_SIZE;
	u16 cnt=0;
	char cmd[16];
	
	while(loop--){
		
		if(esp8266_cmd("AT+CIPSEND=0,2048") == 0){
			for(uint16_t j = 0; j < ESP8266_MAX_SIZE; j++)//循环发送数据
			{
				while(USART_GetFlagStatus(ESP8266_USART,USART_FLAG_TC)==RESET);  //等待上次传输完成 
				USART_SendData(ESP8266_USART,(uint8_t)data[cnt * ESP8266_MAX_SIZE + j]); 	 //发送数据到串口3 
			}
		}
		cnt++;
		delay_ms(10);
	}
	
	if(end == 0) return;
	
	sprintf(cmd, "AT+CIPSEND=0,%d", end);
	if(esp8266_cmd(cmd) == 0){
		for(uint16_t j = 0; j < end; j++)//循环发送数据
		{
			while(USART_GetFlagStatus(ESP8266_USART,USART_FLAG_TC)==RESET);  //等待上次传输完成 
			USART_SendData(ESP8266_USART,(uint8_t)data[cnt * ESP8266_MAX_SIZE + j]); 	 //发送数据到串口3 
		}
		delay_ms(10);
	}
	
}
