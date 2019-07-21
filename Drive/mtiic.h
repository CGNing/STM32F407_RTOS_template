#ifndef __MT_IIC_H
#define __MT_IIC_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

//IO方向设置
#define MT_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define MT_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式
//IO操作函数	 
#define MT_IIC_SCL    PBout(8) //SCL
#define MT_IIC_SDA    PBout(9) //SDA	 
#define MT_READ_SDA   PBin(9)  //输入SDA 

//MT_IIC所有操作函数
void MT_IIC_Init(void);                //初始化MT_IIC的IO口				 
void MT_IIC_Start(void);				//发送MT_IIC开始信号
void MT_IIC_Stop(void);	  			//发送MT_IIC停止信号
void MT_IIC_Send_Byte(u8 txd);			//MT_IIC发送一个字节
u8 MT_IIC_Read_Byte(unsigned char ack);//MT_IIC读取一个字节
s8 MT_IIC_Wait_Ack(void); 				//MT_IIC等待ACK信号
void MT_IIC_Ack(void);					//MT_IIC发送ACK信号
void MT_IIC_NAck(void);				//MT_IIC不发送ACK信号

#endif
