/************************GUO NING*******************
���ߣ�CGN
�ļ�����ad9854.c
�������ڣ�2017.7.26
ά���ˣ�
ά�����ڣ�
������AD9854����
�ܽţ�SCLK:PB13,SPI2_CLK
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
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTAʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
 

}

//void AD9854_WriteIntr(u8 cmd){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){ //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		retry++;
//		if(retry>200) return;	
//	}	  
//	SPI_I2S_SendData(SPI2, cmd&0X0F); //ͨ������SPIx����һ������
//}

//void AD9854_ReadIntr(u8 cmd){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){ //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		retry++;
//		if(retry>200) return;
//	}
//		retry++;		  
//	SPI_I2S_SendData(SPI2, (cmd&0X0F)|0X80); //ͨ������SPIx����һ������
//}


//u8 AD9854_ReadWriteByte(u8 TxData){
//	u8 retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}			  
//	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
//	retry=0;

//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
//		{
//		retry++;
//		if(retry>200)return 0;
//		}	  						    
//	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
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
	
	RESET_H;	//��λ
	delay_ms(1);
	RESET_L;
	
	AD9854_CS_L;
	delay_us(1);
	AD9854_WriteIntr(ADDR_CR);
	AD9854_ReadWriteByte(0X10);	//�رձȽ���
	AD9854_ReadWriteByte(0X40|CLK_SET);	//����ʱ�ӱ�Ƶ
	AD9854_ReadWriteByte(0X00);	//����ϵͳΪģʽ0,�ⲿ����
	AD9854_ReadWriteByte(0X21);	//����Ϊ�ɵ�����,ȡ����ֵ����
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
	RESET_H;	//��λ
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
	RESET_H;	//��λ
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
