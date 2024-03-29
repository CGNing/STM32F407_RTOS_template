#include "mtiic.h"

//初始化MT_IIC
void MT_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	MT_IIC_SCL=1;
	MT_IIC_SDA=1;
}
//产生MT_IIC起始信号
void MT_IIC_Start(void)
{
	MT_SDA_OUT();     //sda线输出
	MT_IIC_SDA=1;	  	  
	MT_IIC_SCL=1;
	delay_us(4);
 	MT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MT_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生MT_IIC停止信号
void MT_IIC_Stop(void)
{
	MT_SDA_OUT();//sda线输出
	MT_IIC_SCL=0;
	MT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MT_IIC_SCL=1; 
	MT_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
s8 MT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MT_SDA_IN();      //SDA设置为输入  
	MT_IIC_SDA=1;delay_us(1);	   
	MT_IIC_SCL=1;delay_us(1);	 
	while(MT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MT_IIC_Stop();
			return -1;
		}
	}
	MT_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void MT_IIC_Ack(void)
{
	MT_IIC_SCL=0;
	MT_SDA_OUT();
	MT_IIC_SDA=0;
	delay_us(2);
	MT_IIC_SCL=1;
	delay_us(2);
	MT_IIC_SCL=0;
}
//不产生ACK应答		    
void MT_IIC_NAck(void)
{
	MT_IIC_SCL=0;
	MT_SDA_OUT();
	MT_IIC_SDA=1;
	delay_us(2);
	MT_IIC_SCL=1;
	delay_us(2);
	MT_IIC_SCL=0;
}					 				     
//MT_IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MT_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	MT_SDA_OUT(); 	    
  MT_IIC_SCL=0;//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
		MT_IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		MT_IIC_SCL=1;
		delay_us(2); 
		MT_IIC_SCL=0;	
		delay_us(2);
   }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MT_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
    MT_IIC_SCL=0; 
    delay_us(2);
		MT_IIC_SCL=1;
    receive<<=1;
    if(MT_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
			MT_IIC_NAck();//发送nACK
    else
			MT_IIC_Ack(); //发送ACK   
    return receive;
}
