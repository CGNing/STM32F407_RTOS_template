/*******************************
���ߣ�CGN
�ļ�����ov2640_dcmi.c
�������ڣ�2018.6.5
������ov2640����DCMI
********************************/
#include "ov2640_dcmi.h"

/**
   @brief      DCMI��ʼ��
   @param      NULL
   @return     void					
   @since      v1.0
*/
void dcmi_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA B C E ʱ��
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;//PC6/7/8/9/11 ���ù������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI); //PC7,AF13  DCMI_D1 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI); //PC8,AF13  DCMI_D2
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_DCMI); //PC9,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_DCMI);//PC11,AF13 DCMI_D4 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_DCMI); //PE5,AF13  DCMI_D6
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7

	DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_SnapShot;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_Init(&DCMI_InitStructure);
	
	DCMI_Cmd(ENABLE);
  
//  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x02;		//�����ȼ�2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);
//	
//	DCMI_ITConfig(DCMI_IT_VSYNC,ENABLE);
	
}

//DCMI_CaptureCmd(ENABLE);

/**
   @brief      DCMI��DMA��ʼ��
   @param      NULL
   @return     void
   @since      v1.0
*/
void dcmi_dma_init(u32 DMA_Memory0BaseAddr,u32 DMA_MemoryDataSize){
	u32 t=0;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
	
	t=0;
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE&&t++<=144000);//�ȴ�DMA2_Stream1������ 
	
	if(t>168000){
		#ifdef PRINTF
		printf("DCMI DMA init error!");
		#endif
		return;
	}
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //ͨ��1 DCMIͨ�� 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;//�����ַΪ:DCMI->DR
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//�洢�����ݳ��� 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_BufferSize = DMA_MemoryDataSize;//���ݴ�����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���

	DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x03;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	
	
}

/**
	@brief		DMA2_Stream1�жϺ���
	@return		void
	@since		v1.0
*/
void DMA2_Stream1_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)!=RESET){
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}
