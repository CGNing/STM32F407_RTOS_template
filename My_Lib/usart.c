/************************GUO NING*******************
作者：CGN
文件名：usart.c
创建日期：2019.7.20
维护人：
维护日期：
描述：
管脚：
	U1 TX A9 
	U1 RX A10
	U2 TX A2
	U2 RX A3
	U3_TX C10
	U3_RX C11
****************************************************/

#include "usart.h"

#include <stdio.h>

//int fputc(int ch,FILE *p) 
//{
//USART_SendData(USART1,(u8)ch);
//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//return ch;
//}

void USART1_Init(u32 baud){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStructure);
	
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	USART_ClearFlag(USART1,USART_FLAG_TC);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
}

void USART2_Init(u32 baud){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	
	USART_Init(USART2,&USART_InitStructure);
	
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	USART_ClearFlag(USART2,USART_FLAG_TC);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//强占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;//相应优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
	
}

void USART_Send_Byte(USART_TypeDef* USARTx,u8 data){
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	USART_SendData(USARTx,0X01);	
}

void USART_Send_nByte(USART_TypeDef* USARTx,u8* data){
	u32 u=sizeof(*data),i=0;
	for(;i<=u;i++){
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(USARTx,data[i]);
	}
}

void USART_Send_Str(USART_TypeDef* USARTx,u8* data){
	while((*data<='~')&&(*data>=' ')){
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(USARTx,*data);
		data++;
	}
}

void USART_Send_Num(USART_TypeDef* USARTx,int Num){
	u8 enshow=0;
	u8 i,u,temp;
	u32 pow;
	if(Num<0){
		Num=-Num;
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(USARTx,'-');	
	}
	for(i=0;i<10;i++)
	{
		pow=1;
		for(u=0;u<9-i;u++)
			pow*=10;
		temp=(Num/pow)%10;
		if(enshow==0&&i<9)
		{
			if(temp==0)
				continue;
			else enshow=1; 
		 	 
		}
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	 	USART_SendData(USARTx,temp+'0');
	}	
}
