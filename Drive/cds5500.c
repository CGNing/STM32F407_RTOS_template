/************************GUO NING*******************
���ߣ�CGN
�ļ�����cds5500.c
�������ڣ�2017.7.19
ά���ˣ�
ά�����ڣ�
���������ֶ������
�ܽţ�TX3,PB10,U3_TX
			RX3,PB11,U3_RX
****************************************************/

#include "cds5500.h"

struct _CDS5500_data{
	u8 len;							//֡����
	u8 buf[20];			//֡����
}CDS5500_data;

void UART_DMA_conf(void);

void CDS_conf(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	//ʹ�ܴ���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
// B10 ��T3X
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   //���츴�����
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// B11 ��R3X 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

#ifdef BOTHWAY
	
	UART3_RXDS;
 	UART3_TXDS;
	//C5��TX3EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//C4��RX3EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
#endif
	
	USART_InitStructure.USART_BaudRate = 1000000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockInit(USART3, &USART_ClockInitStructure);
	USART_Init(USART3, &USART_InitStructure);
    
  USART_Cmd(USART3, ENABLE);
	//����3ʹ�ý����ж�
 //	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);// */
	UART_DMA_conf();
}

void UART_DMA_conf(void){
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART3->DR));			//ԴͷBUF
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)CDS5500_data.buf;									//Ŀ��BUF
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							//������Ŀ��
	DMA_InitStructure.DMA_BufferSize = 0;										//BUF��С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//�����ֽ�Ϊ��λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;			//�ڴ��ֽ�Ϊ��λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//��ͨģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//4���ȼ�֮һ��(������)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
//	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);								//DMA2��������ж�
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);								//����3��������DMA
	
}

void CDS_cmd(u8 id, u8 len, u8 cmd, u8* pData){
	u8 i=0,csum=0;
	u16 pos=0;
	
	CDS5500_data.buf[pos++]=0XFF;
	CDS5500_data.buf[pos++]=0XFF;
	CDS5500_data.buf[pos++]=id;
	CDS5500_data.buf[pos++]=len;
	CDS5500_data.buf[pos++]=cmd;
	csum=id+len+cmd;
	for(i=0; i<len-2; i++)
	{
		CDS5500_data.buf[pos++]=pData[i];
		csum+=pData[i];
	}
	CDS5500_data.buf[pos++]=~csum;
	CDS5500_data.len=pos;
	
	UART3_RXDS;
	UART3_TXEN;
	
	DMA1_Channel2->CMAR = (u32)CDS5500_data.buf;
	DMA1_Channel2->CNDTR = CDS5500_data.len;		//Ҫ���͵��ֽ���
	DMA1_Channel2->CCR |= 0x01;										//����һ��DMA 
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC2)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC2);				//����жϱ�־
	DMA_Cmd(DMA1_Channel2,DISABLE);		//����DMA
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	UART3_TXDS;
	UART3_RXEN;
	delay_us(100);
}

void CDS_SetMode(u8 id,u8 mode){
	u8 buf[5];
	if(mode > 1)
		return;
	buf[0] = 0x06;
	if(mode == CDS_SEVMODE)
	{
		buf[1] = 0;		
		buf[2] = 0;
		buf[3] = 0xff;
		buf[4] = 0x03;
	}
	else if(mode == CDS_MOTOMODE)
	{
		buf[1] = 0;		
		buf[2] = 0;
		buf[3] = 0;
		buf[4] = 0;
	}
	CDS_cmd(id,0x07,CMD_WRITE,buf);
}

void CDS_SetAngle(u8 id,u16 angle,u16 speed){
	u8 buf[5];
	if(angle > 1023 || speed > 1023)
		return;
	buf[0] = 0x1E;
	buf[1] = angle;		
	buf[2] = angle>>8;
	buf[3] = speed;
	buf[4] = speed>>8;
	CDS_cmd(id,0x07,CMD_WRITE,buf);
}

void CDS_SetSpeed(u8 id,s16 speed){
	u8 buf[5];
	u16 temp;
	if(speed > 1023)	speed=1023;
	if(speed<-1023)	speed=-1023;
	
	temp = speed>0?speed:-speed;
	buf[0] = 0x20;
	buf[1] = temp;
	buf[2] = temp>>8;
	if(speed < 0)
		buf[2] |= 0x04;
	CDS_cmd(id,0x05,CMD_WRITE,buf);
}
