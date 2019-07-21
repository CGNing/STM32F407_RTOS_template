#ifndef __MT_IIC_H
#define __MT_IIC_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

//IO��������
#define MT_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define MT_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define MT_IIC_SCL    PBout(8) //SCL
#define MT_IIC_SDA    PBout(9) //SDA	 
#define MT_READ_SDA   PBin(9)  //����SDA 

//MT_IIC���в�������
void MT_IIC_Init(void);                //��ʼ��MT_IIC��IO��				 
void MT_IIC_Start(void);				//����MT_IIC��ʼ�ź�
void MT_IIC_Stop(void);	  			//����MT_IICֹͣ�ź�
void MT_IIC_Send_Byte(u8 txd);			//MT_IIC����һ���ֽ�
u8 MT_IIC_Read_Byte(unsigned char ack);//MT_IIC��ȡһ���ֽ�
s8 MT_IIC_Wait_Ack(void); 				//MT_IIC�ȴ�ACK�ź�
void MT_IIC_Ack(void);					//MT_IIC����ACK�ź�
void MT_IIC_NAck(void);				//MT_IIC������ACK�ź�

#endif
