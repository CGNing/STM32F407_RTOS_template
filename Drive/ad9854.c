/************************GUO NING*******************
作者：CGN
文件名：ad9854.c
创建日期：2017.7.26
维护人：
维护日期：
描述：AD9854配置
管脚：SCLK:PB13,SPI2_CLK
			MISO:PB14,SPI2_MISO
			MOSI:PB15,SPI2_MOSI
			CS:PB12
			RESET:D10
			UDCLK:D11
			FDATA:
			OSK:
****************************************************/

#include "ad9854.h"

const double Freq_mult_double = 2814749.76710656;

void AD9854_SPI2_Init(void){
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTA时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
 

}

//void AD9854_WriteIntr(u8 cmd){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){ //检查指定的SPI标志位设置与否:发送缓存空标志位
//		retry++;
//		if(retry>200) return;	
//	}	  
//	SPI_I2S_SendData(SPI2, cmd&0X0F); //通过外设SPIx发送一个数据
//}

//void AD9854_ReadIntr(u8 cmd){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){ //检查指定的SPI标志位设置与否:发送缓存空标志位
//		retry++;
//		if(retry>200) return;
//	}
//		retry++;		  
//	SPI_I2S_SendData(SPI2, (cmd&0X0F)|0X80); //通过外设SPIx发送一个数据
//}


//u8 AD9854_ReadWriteByte(u8 TxData){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
//		{
//		retry++;
//		if(retry>200)return 0;
//		}			  
//	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
//		{
//		retry++;
//		if(retry>200)return 0;
//		}	  						    
//	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
//}

void AD9854_WriteIntr(u8 cmd){
	AD9854_ReadWriteByte(cmd&0X0F);
}

void AD9854_ReadIntr(u8 cmd){
	AD9854_ReadWriteByte((cmd&0X0F)|0X80);
}

u8 AD9854_ReadWriteByte(u8 TxData){
	u8 i,u,temp;
	
	for(u=0;u<2;u++);
	
	for(i=0;i<8;i++){
		AD9854_CLK_L;
		if(TxData&0X80) AD9854_MOSI_H;
		else AD9854_MOSI_L;
		TxData=TxData<<1;
		for(u=0;u<2;u++);
		AD9854_CLK_H;
		for(u=0;u<2;u++);
		temp=temp<<1;
		temp|=AD9854_MISO;
	}
	return temp;
}



void AD9854_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );
	
//	AD9854_SPI2_Init();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	MASTER_RESET_L;
	
	AD9854_CS_H;
	AD9854_CLK_L;
	AD9854_MOSI_H;
	
	RESET_H;	//复位
	delay_ms(1);
	RESET_L;
	
	AD9854_CS_L;
	delay_us(1);
	AD9854_WriteIntr(ADDR_CR);
	AD9854_ReadWriteByte(0X10);	//关闭比较器
	AD9854_ReadWriteByte(0X40|CLK_SET);	//设置时钟倍频
	AD9854_ReadWriteByte(0X00);	//设置系统为模式0,外部更新
	AD9854_ReadWriteByte(0X21);	//设置为可调幅度,取消差值补偿
	delay_us(1);
	AD9854_CS_H;
	
	delay_ms(10);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	UDCLK_H;
	delay_ms(1);
	UDCLK_L;
	
}

void AD9854_Freq_doublt_convent(unsigned long Freq,u8* buf){
	unsigned long Low32;
	unsigned int High16;
	double Temp=Freq_mult_double;
	Freq*=(double)(Temp);
	High16=(int)(Freq/4294967295);	//2^32
	Freq-=(double)High16*4294967295;
	Low32=(unsigned long)Freq;
	
	buf[0]=Low32;
	buf[1]=Low32>>8;
	buf[2]=Low32>>16;
	buf[3]=Low32>>24;
	buf[4]=High16;
	buf[5]=High16>>8;
	
}

void AD9854_SetSinFreq(u32 Freq,u16 Shape){
	u8 buf[6];
	uint64_t FTW;
	FTW=Freq/(float)(OSC*CLK_SET)*281474976710656;
	buf[0]=FTW;
	buf[1]=FTW>>8;
	buf[2]=FTW>>16;
	buf[3]=FTW>>24;
	buf[4]=FTW>>32;
	buf[5]=FTW>>40;
//	
//	buf[0]=0XCC;
//	buf[1]=0XCC;
//	buf[2]=0XCC;
//	buf[3]=0XCC;
//	buf[4]=0XCC;
//	buf[5]=0X0C;
	
	
	UDCLK_L;
	RESET_H;	//复位
	delay_ms(1);
	RESET_L;
	
	AD9854_CS_L;
	delay_us(10);
	AD9854_WriteIntr(ADDR_FREQ1);
	AD9854_ReadWriteByte(buf[5]);
	AD9854_ReadWriteByte(buf[4]);
	AD9854_ReadWriteByte(buf[3]);
	AD9854_ReadWriteByte(buf[2]);
	AD9854_ReadWriteByte(buf[1]);
	AD9854_ReadWriteByte(buf[0]);
	delay_us(10);
	AD9854_CS_H;
	
	UDCLK_H;
	delay_us(500);
	UDCLK_L;
	delay_ms(1);
	RESET_H;	//复位
	delay_us(500);
	RESET_L;
	
	AD9854_CS_L;
	delay_us(10);
	AD9854_WriteIntr(ADDR_IPDMR);
	AD9854_ReadWriteByte(Shape>>8);
	AD9854_ReadWriteByte(Shape);
	delay_us(10);
	AD9854_CS_H;
	
	UDCLK_H;
	delay_us(500);
	UDCLK_L;
}
