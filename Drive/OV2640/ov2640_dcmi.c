/*******************************
作者：CGN
文件名：ov2640_dcmi.c
创建日期：2018.6.5
描述：ov2640配置DCMI
********************************/
#include "ov2640_dcmi.h"

/**
   @brief      DCMI初始化
   @param      NULL
   @return     void					
   @since      v1.0
*/
void dcmi_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA B C E 时钟
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;//PC6/7/8/9/11 复用功能输出
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

	DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_SnapShot;//连续模式
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//全帧捕获
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8位数据格式  
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK 上升沿有效
	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC 低电平有效
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC 低电平有效
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//硬件同步HSYNC,VSYNC
	DCMI_Init(&DCMI_InitStructure);
	
	DCMI_Cmd(ENABLE);
  
//  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x02;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	DCMI_ITConfig(DCMI_IT_VSYNC,ENABLE);
	
}

//DCMI_CaptureCmd(ENABLE);

/**
   @brief      DCMI的DMA初始化
   @param      NULL
   @return     void
   @since      v1.0
*/
void dcmi_dma_init(u32 DMA_Memory0BaseAddr,u32 DMA_MemoryDataSize){
	u32 t=0;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
	
	t=0;
	while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE&&t++<=144000);//等待DMA2_Stream1可配置 
	
	if(t>168000){
		#ifdef PRINTF
		printf("DCMI DMA init error!");
		#endif
		return;
	}
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //通道1 DCMI通道 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;//外设地址为:DCMI->DR
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//外设数据长度:32位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;//DMA 存储器0地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//存储器数据长度 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_BufferSize = DMA_MemoryDataSize;//数据传输量
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用循环模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFO模式        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//使用全FIFO 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//外设突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//存储器突发单次传输

	DMA_Init(DMA2_Stream1, &DMA_InitStructure);//初始化DMA Stream

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x03;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);
	
	
}

/**
	@brief		DMA2_Stream1中断函数
	@return		void
	@since		v1.0
*/
void DMA2_Stream1_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)!=RESET){
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}
