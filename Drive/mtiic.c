#include "mtiic.h"

//��ʼ��MT_IIC
void MT_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	MT_IIC_SCL=1;
	MT_IIC_SDA=1;
}
//����MT_IIC��ʼ�ź�
void MT_IIC_Start(void)
{
	MT_SDA_OUT();     //sda�����
	MT_IIC_SDA=1;	  	  
	MT_IIC_SCL=1;
	delay_us(4);
 	MT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MT_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����MT_IICֹͣ�ź�
void MT_IIC_Stop(void)
{
	MT_SDA_OUT();//sda�����
	MT_IIC_SCL=0;
	MT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MT_IIC_SCL=1; 
	MT_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
s8 MT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MT_SDA_IN();      //SDA����Ϊ����  
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
	MT_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//MT_IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MT_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	MT_SDA_OUT(); 	    
  MT_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
		MT_IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		MT_IIC_SCL=1;
		delay_us(2); 
		MT_IIC_SCL=0;	
		delay_us(2);
   }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MT_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MT_SDA_IN();//SDA����Ϊ����
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
			MT_IIC_NAck();//����nACK
    else
			MT_IIC_Ack(); //����ACK   
    return receive;
}
