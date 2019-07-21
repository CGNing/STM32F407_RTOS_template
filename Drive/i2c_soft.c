/************************GUO NING*******************
作者：CGN
文件名：i2c_soft.c
创建日期：2017.8.31
维护人：
维护日期：
描述：I2C软件管脚模拟实现
管脚：
****************************************************/

#include "i2c_soft.h"

volatile u8 I2C_FastMode;

void I2c_Soft_delay()
{ 
	__nop();__nop();__nop();
	__nop();__nop();__nop();
	__nop();__nop();__nop();
	
	if(!I2C_FastMode)
	{
		u8 i = 15;
		while(i--);
	}
}

void I2c_Soft_Init()
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
  RCC_AHB1PeriphClockCmd(I2C_SCL_RCC, ENABLE );
	RCC_AHB1PeriphClockCmd(I2C_SDA_RCC, ENABLE );
	
  GPIO_InitStructure.GPIO_Pin =  I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStructure);			
	
	GPIO_InitStructure.GPIO_Pin =  I2C_SDA_PIN;
	GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStructure);
}

int I2c_Soft_Start()
{
	I2C_SDA_H;
	I2C_SCL_H;
	I2c_Soft_delay();
	if(!I2C_SDA_read)return 0;
	I2C_SDA_L;
	I2c_Soft_delay();
	if(I2C_SDA_read) return 0;
	I2C_SDA_L;
	I2c_Soft_delay();
	return 1;	

}

void I2c_Soft_Stop()
{
	I2C_SCL_L;
	I2c_Soft_delay();
	I2C_SDA_L;
	I2c_Soft_delay();
	I2C_SCL_H;
	I2c_Soft_delay();
	I2C_SDA_H;
	I2c_Soft_delay();
}

void I2c_Soft_Ask()
{
	I2C_SCL_L;
	I2c_Soft_delay();
	I2C_SDA_L;
	I2c_Soft_delay();
	I2C_SCL_H;
	I2c_Soft_delay();
	I2C_SCL_L;
	I2c_Soft_delay();
}

void I2c_Soft_NoAsk()
{
	I2C_SCL_L;
	I2c_Soft_delay();
	I2C_SDA_H;
	I2c_Soft_delay();
	I2C_SCL_H;
	I2c_Soft_delay();
	I2C_SCL_L;
	I2c_Soft_delay();
}

int I2c_Soft_WaitAsk(void)
{
  u8 ErrTime = 0;
	I2C_SCL_L;
	I2c_Soft_delay();
	I2C_SDA_H;			
	I2c_Soft_delay();
	I2C_SCL_H;
	I2c_Soft_delay();
	while(I2C_SDA_read)
	{
		ErrTime++;
		if(ErrTime>50)
		{
			I2c_Soft_Stop();
			return 1;
		}
	}
	I2C_SCL_L;
	I2c_Soft_delay();
	return 0;
}

void I2c_Soft_SendByte(u8 SendByte)
{
    u8 i=8;
    while(i--)
    {
        I2C_SCL_L;
        I2c_Soft_delay();
      if(SendByte&0x80)
        I2C_SDA_H;  
      else 
        I2C_SDA_L;   
        SendByte<<=1;
        I2c_Soft_delay();
				I2C_SCL_H;
				I2c_Soft_delay();
    }
    I2C_SCL_L;
}  

//?1???,ack=1?,??ACK,ack=0,??NACK
u8 I2c_Soft_ReadByte(u8 ask)  //????????//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    I2C_SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      I2C_SCL_L;
      I2c_Soft_delay();
			I2C_SCL_H;
      I2c_Soft_delay();	
      if(I2C_SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    I2C_SCL_L;

	if (ask)
		I2c_Soft_Ask();
	else
		I2c_Soft_NoAsk();  
    return ReceiveByte;
} 


// IIC???????
u8 IIC_Write_1Byte(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1);   
	if(I2c_Soft_WaitAsk())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address);       
	I2c_Soft_WaitAsk();	
	I2c_Soft_SendByte(REG_data);
	I2c_Soft_WaitAsk();   
	I2c_Soft_Stop(); 
	return 0;
}

// IIC?1????
u8 IIC_Read_1Byte(u8 SlaveAddress,u8 REG_Address,u8 *REG_data)
{      		
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address);     
	I2c_Soft_WaitAsk();
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1 | 0x01);
	I2c_Soft_WaitAsk();
	*REG_data= I2c_Soft_ReadByte(0);
	I2c_Soft_Stop();
	return 0;
}	

// IIC?n????
u8 IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{	
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address); 
	I2c_Soft_WaitAsk();
	while(len--) 
	{
		I2c_Soft_SendByte(*buf++); 
		I2c_Soft_WaitAsk();
	}
	I2c_Soft_Stop();
	return 0;
}

// IIC?n????
u8 IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u8 len, u8 *buf)
{	
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1); 
	if(I2c_Soft_WaitAsk())
	{
		I2c_Soft_Stop();
		return 1;
	}
	I2c_Soft_SendByte(REG_Address); 
	I2c_Soft_WaitAsk();
	
	I2c_Soft_Start();
	I2c_Soft_SendByte(SlaveAddress<<1 | 0x01); 
	I2c_Soft_WaitAsk();
	while(len) 
	{
		if(len == 1)
		{
			*buf = I2c_Soft_ReadByte(0);
		}
		else
		{
			*buf = I2c_Soft_ReadByte(1);
		}
		buf++;
		len--;
	}
	I2c_Soft_Stop();
	return 0;
}
