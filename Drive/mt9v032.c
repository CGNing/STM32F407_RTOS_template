/***********************************************
作者：CGN
文件名：mt9v032.c
创建日期：2017.7.19
维护人：
维护日期：
描述：总钻风摄像头控制
接线：  模块管脚            单片机管脚
			  SDA                 A2
			  SCL                 A3		  
        行中断              A4 
				场中断              B7				
			  像素中断            A6                   
			  D0                  C6
        D1                  C7
        D2                  C8
        D3                  C9
        D4                  C11
        D5                  B6
        D6                  E5
        D7                  E6
****************************************************/
#include "mt9v032.h"

#include <stdio.h>

#define WR_MODE 0XB8
#define RD_MODE 0XB9

#define MT9V032_REG_CHIP_VERSION					0x00		
#define MT9V032_REG_COLUMN_START					0X01	//1-752 W
#define MT9V032_REG_ROW_START							0X02	//4-482 W
#define MT9V032_REG_WIN_HEIGHT						0X03	//1-480 W
#define MT9V032_REG_WIN_WIDTH							0X04	//1-752 W
#define MT9V032_REG_HER_BLANKING					0X05	//43-1023 W
#define MT9V032_REG_VER_BLANKING					0X06	//4-3000 W
#define MT9V032_REG_CHIP_CTRL							0X07	
#define MT9V032_REG_SHUTTER_WIDTH_1				0X08	//1-32767 W	
#define MT9V032_REG_SHUTTER_WIDTH_2				0X09	//1-32767 W
#define MT9V032_REG_SHUTTER_WIDTH_CTRL		0X0A
#define MT9V032_REG_TOTAL_SHUTTER_WIDTH		0X0B	//1-32767 W
#define MT9V032_REG_RESET									0X0C
#define MT9V032_REG_READ_MODE							0X0D
#define MT9V032_REG_MONITOR_MODE					0X0E	//0,1 W
#define MT9V032_REG_PIXEL_OPERATION_MODE	0X0F	//2 0=Monochrome.1=Color;6 0=Linear operation.1=High Dynamic Range. W
#define MT9V032_REG_LED_OUTCTRL						0X1B	//0,1 W
#define RED_ADC_MODE_CTRL									0X1C	//1:0 2=10-bit linear.3=12-to10-bit companding. W
#define MT9V032_REG_VREF_ADC_CTRL					0X2C	//2:0 0=1.0V 1=1.1V 2=1.2V 3=1.3V 4=1.4V 5=1.5V 6=1.6V 7=2.1V W
#define MT9V032_REG_V1										0X31	//0-31 V_Step=bits(4:0)x62.5mV+0.5625V. Range:0.5625 - 2.5V; Default: 2.375V. W
#define MT9V032_REG_V2										0X32
#define MT9V032_REG_V3										0X33
#define MT9V032_REG_V4										0X34
#define MT9V032_REG_ANALOG_GAIN						0X35	//16-64 Analog gain=bits(6:0)x0.0625 for values 16-31;Analog gain=bits(6:0)/2 x 0.125 for values 32-64 W
#define MT9V032_REG_MAX_ABALOG_GAIN				0X36	//16-64 W
#define MT9V032_REG_FRAME_DARK_AVG				0X42	//R
#define MT9V032_REG_DARK_AVG_THRSHOLDS		0X46	
#define MT9V032_REG_BL_CALIB_CTRL					0X47
#define MT9V032_REG_BL_CALIB_VALUE				0X48	//-127-127 W
#define MT9V032_REG_BL_CALIB_STEP_SIZE		0X4C	//0-31
#define MT9V032_REG_NOISE_CORR_CTRL_1			0X70	
#define MT9V032_REG_NOISE_CONSTANT				0X72	//0-255 W	
#define MT9V032_REG_NOISE_CORR_CTRL_2			0X73	//759-775 Dark startcolumn address W
#define MT9V032_REG_PIXCLK_FV_LV					0X74
#define MT9V032_REG_DIG_TEST_PATTERN			0X7F
#define MT9V032_REG_TILE_WE_GAIN_X0_Y0		0X80
#define MT9V032_REG_TILE_WE_GAIN_X1_Y0		0X81
#define MT9V032_REG_TILE_WE_GAIN_X2_Y0		0X82
#define MT9V032_REG_TILE_WE_GAIN_X3_Y0		0X83
#define MT9V032_REG_TILE_WE_GAIN_X4_Y0		0X84
#define MT9V032_REG_TILE_WE_GAIN_X0_Y1		0X85
#define MT9V032_REG_TILE_WE_GAIN_X1_Y1		0X86
#define MT9V032_REG_TILE_WE_GAIN_X2_Y1		0X87
#define MT9V032_REG_TILE_WE_GAIN_X3_Y1		0X88
#define MT9V032_REG_TILE_WE_GAIN_X4_Y1		0X89
#define MT9V032_REG_TILE_WE_GAIN_X0_Y2		0X8A
#define MT9V032_REG_TILE_WE_GAIN_X1_Y2		0X8B
#define MT9V032_REG_TILE_WE_GAIN_X2_Y2		0X8C
#define MT9V032_REG_TILE_WE_GAIN_X3_Y2		0X8D
#define MT9V032_REG_TILE_WE_GAIN_X4_Y2		0X8E
#define MT9V032_REG_TILE_WE_GAIN_X0_Y3		0X8F
#define MT9V032_REG_TILE_WE_GAIN_X1_Y3		0X90
#define MT9V032_REG_TILE_WE_GAIN_X2_Y3		0X91
#define MT9V032_REG_TILE_WE_GAIN_X3_Y3		0X92
#define MT9V032_REG_TILE_WE_GAIN_X4_Y3		0X93
#define MT9V032_REG_TILE_WE_GAIN_X0_Y4		0X94
#define MT9V032_REG_TILE_WE_GAIN_X1_Y4		0X95
#define MT9V032_REG_TILE_WE_GAIN_X2_Y4		0X96
#define MT9V032_REG_TILE_WE_GAIN_X3_Y4		0X97
#define MT9V032_REG_TILE_WE_GAIN_X4_Y4		0X98
#define MT9V032_REG_TILE_COORD_X_0_5			0X99 //0-752 W
#define MT9V032_REG_TILE_COORD_X_1_5			0X9A
#define MT9V032_REG_TILE_COORD_X_2_5			0X9B
#define MT9V032_REG_TILE_COORD_X_3_5			0X9C
#define MT9V032_REG_TILE_COORD_X_4_5			0X9D
#define MT9V032_REG_TILE_COORD_X_5_5			0X9E
#define MT9V032_REG_TILE_COORD_Y_0_5			0X9F	//0-480 W
#define MT9V032_REG_TILE_COORD_Y_1_5			0XA0
#define MT9V032_REG_TILE_COORD_Y_2_5			0XA1
#define MT9V032_REG_TILE_COORD_Y_3_5			0XA2
#define MT9V032_REG_TILE_COORD_Y_4_5			0XA3
#define MT9V032_REG_TILE_COORD_Y_5_5			0XA4
#define MT9V032_REG_AEC_AGC_DESIRED_BIN		0XA5	//1-64 W
#define MT9V032_REG_AEC_UPDATE_FREQ				0XA6	//0-15 W
#define MT9V032_REG_AEC_LPF								0XA8	//0-2 W
#define MT9V032_REG_AGC_UPDATE_FREQ				0XA9	//0-15 W
#define MT9V032_REG_AGC_LPF								0XAB	//0-2 W
#define MT9V032_REG_AGC_AEC_EN						0XAF	//0 AEC,1 AGC W
#define MT9V032_REG_AEC_AGC_PIX_COUNT			0XB0	//0-65535 W
#define MT9V032_REG_LVDS_MASTER_CTRL			0XB1	//0 PLL Bypass,1 LVDS Powerdown,2 PLL Test Mode,3 LVDS Test Mode
#define MT9V032_REG_LVDS_SHIFT_CLK_CTRL		0XB2	//2:0 Shift-clk Delay Element Select,4 LVDS Receiver Power-down W
#define MT9V032_REG_LVDS_DATA_CTRL				0XB3	//2:0 Data Delay Element Select,4 LVDS Driver Power-down W
#define MT9V032_REG_DATA_STREAM_LATENCY		0XB4	//0-3 W
#define MT9V032_REG_LVDS_INTERAL_SYNC			0XB5	//0,1 W
#define MT9V032_REG_LVDS_PAYLOAD_CTRL			0XB6	//Use 10-bit Pixel Enable 0,1 W
#define MT9V032_REG_STEREOSCOP_ERROR_CTRL	0XB7	//0 Enable Stereo Error Detect,1 Enable Stick Stereo Error Flag,2 Claer Stereo Error Flag
#define MT9V032_REG_STEREOSCOP_ERROR_FLAG	0XB8	//Stereoscopy Error Flag R
#define MT9V032_REG_LVDS_DATA_OUTPUT			0XB9	//Combo Reg R
#define MT9V032_REG_AGC_GAIN_OUTPUT				0XBA	//AGC Gain R
#define MT9V032_REG_AEC_GAIN_OUTPUT				0XBB	//AEC Exposure R
#define MT9V032_REG_AGC_AEC_CURRENT_BIN		0XBC	//Current Bin R
#define MT9V032_REG_MAX_SHUTTER_WIDTH			0XBD	//1-2047 W
#define MT9V032_REG_AGC_AEC_BIN_DIFF_TH		0XBE	//0-63 W
#define MT9V032_REG_FIELD_BLANK						0XBF	//0-255 W
#define MT9V032_REG_MON_MODE_CAPTURE_CTRL	0XC0	//0-255 W
#define MT9V032_REG_TEMPERTURE						0XC1	//Temperature Output R
#define MT9V032_REG_ANALOG_CTRL						0XC2	//0 Extend Frame Valid,1 Replace FV/LV with Ped/Snyc W
#define MT9V032_REG_NIFS_FV_LV_CTRL				0XC3	
#define MT9V032_REG_NIFS_HORIZ_BLANK_CTRL	0XC4
#define MT9V032_REG_NIFS_VERT_BLANK_CTRL	0XC5
#define MT9V032_REG_BYTEWISE_ADDR					0XF0
#define MT9V032_REG_REGISTER_LOCK					0XFE	//unlock two-wire serial interface 0XDEAD,two-wire serial interface is locked 0XBEEF,lock Register 13 only 0XDEAF W
#define MT9V032_REG_CHIP_VERSION_2				0XFF

#define MT9V032_PCLK											26660000

#define CFG_NUM		13

u16 MT9V032_REG_CFG[CFG_NUM][2]={
	//FPS=50
	{MT9V032_REG_COLUMN_START,				256},
	{MT9V032_REG_ROW_START,						120},
	{MT9V032_REG_WIN_HEIGHT,					MT9V032_HEIGHT},//320
	{MT9V032_REG_WIN_WIDTH,						MT9V032_WIDTH},//240
	{MT9V032_REG_HER_BLANKING,				460},//460
	{MT9V032_REG_VER_BLANKING,				442},//442
	//FPS=50
	{MT9V032_REG_CHIP_CTRL,						0X0388},
	{MT9V032_REG_TOTAL_SHUTTER_WIDTH,	1000},
	{MT9V032_REG_READ_MODE,						0X0300},
	{MT9V032_REG_MONITOR_MODE,				0X0000},
	{MT9V032_REG_PIXEL_OPERATION_MODE,0X0011},
	{MT9V032_REG_AEC_AGC_DESIRED_BIN,	32},
	{MT9V032_REG_AGC_AEC_EN,					0X0003},	
};

u8 pic_data[MT9V032_WIDTH*MT9V032_HEIGHT];

/**
   @brief      MT9V032摄像头DCMI中断函数
   @param      NULL
   @return     void					
   @since      v1.0
*/
void DCMI_IRQHandler(void){
	 if(DCMI_GetITStatus(DCMI_IT_VSYNC)!=RESET){
		DCMI_ClearITPendingBit(DCMI_IT_VSYNC);
	}
}

/**
   @brief      MT9V032通过IIC写入寄存器值
   @param      reg:寄存器地址
   @param      data:写入数据
   @return     void
   @since      v1.0
*/
s8 MT_IIC_WR_REG(u8 reg,u16 data){
	s8 s=0;
	MT_IIC_Start();
	MT_IIC_Send_Byte(WR_MODE);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Send_Byte(reg);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Send_Byte(data>>8);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Send_Byte(data&0XFF);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Stop();
	return s;
}

/**
   @brief      MT9V032通过IIC读取寄存器值
   @param      reg:寄存器地址
   @return     void	
   @since      v1.0
*/
s8 MT_IIC_RD_REG(u8 reg,u16* data){
	u16 temp=0;
	s8 s=0;
	MT_IIC_Start();
	MT_IIC_Send_Byte(WR_MODE);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Send_Byte(reg);
	if(MT_IIC_Wait_Ack())
		s--;
	MT_IIC_Start();
	MT_IIC_Send_Byte(RD_MODE);
	if(MT_IIC_Wait_Ack())
		s--;
	temp=MT_IIC_Read_Byte(1);
	temp<<=8;
	temp|=MT_IIC_Read_Byte(0);
	MT_IIC_Stop();
	return s;
}

/**
   @brief      设下风头初始化
   @param      NULL
   @return     s8 0成功,-1失败
   @since      v1.0
   @note			 调用该函数前请先初始化uart2
  */
s8 mt9v032_Init(void){
	u8 i=0;
	MT_IIC_Init();
	for(i=0;i<CFG_NUM;i++){
		MT_IIC_WR_REG(MT9V032_REG_CFG[i][0],MT9V032_REG_CFG[i][1]);
	}
	for(i=0;i<CFG_NUM;i++){
		MT_IIC_RD_REG(MT9V032_REG_CFG[i][0],&MT9V032_REG_CFG[i][1]);
	}
	
	dcmi_dma_init();
	dcmi_init();
	return 0;
}

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
  
//  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;//抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x02;		//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);
//	
//	DCMI_ITConfig(DCMI_IT_VSYNC,ENABLE);
	
}

/**
   @brief      DCMI的DMA初始化
   @param      NULL
   @return     void
   @since      v1.0
*/
void dcmi_dma_init(void){
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
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)pic_data;//DMA 存储器0地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//存储器数据长度 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_BufferSize = MT9V032_WIDTH*MT9V032_HEIGHT/4;//数据传输量
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
   @brief      MT9V032使能
   @param      NULL
   @return     void
   @since      v1.0
*/
void mt9v032_cmd(FunctionalState NewState){
	DMA_Cmd(DMA2_Stream1, NewState);
	DCMI_Cmd(NewState);	//DCMI使能
	DCMI_CaptureCmd(NewState);
}

/**
   @brief      获取摄像头固件版本
   @param      NULL
   @return     void
   @since      v1.0
   @note			 调用该函数前请先初始化uart2
  */
u16 mt9v032_get_version(void){
	u16 temp;
	MT_IIC_RD_REG(MT9V032_REG_CHIP_VERSION,&temp);  
  return temp;
}

/**
   @brief      MT9V032帧率设置
   @param      fps:帧率
   @return     s8 0成功
   @since      v1.0
   @note       F=(N+V)*(A+Q)+4
               F=(S+2)*(A+Q)+4
*/
#define MT9V032_FPS_A	MT9V032_REG_CFG[3][1]
#define MT9V032_FPS_Q	MT9V032_REG_CFG[4][1]
#define MT9V032_FPS_N	MT9V032_REG_CFG[2][1]
#define MT9V032_FPS_V	MT9V032_REG_CFG[5][1]
#define MT9V032_FPS_S	MT9V032_REG_CFG[7][1]
s8 mt9v032_set_fps(u8 fps){
	s8 s=0;
	(MT9V032_FPS_A+50)>700?(MT9V032_FPS_Q=50):(MT9V032_FPS_Q=700-MT9V032_FPS_A);
	MT9V032_FPS_V=(short)((((double)MT9V032_PCLK/fps)-4)/(MT9V032_FPS_A+MT9V032_FPS_Q)-MT9V032_FPS_N);
	//目前设置无法达到帧率时
	if((short)MT9V032_FPS_V<4||(MT9V032_FPS_N+MT9V032_FPS_V-2)<MT9V032_FPS_S){
		(MT9V032_FPS_A+43)>660?(MT9V032_FPS_Q=43):(MT9V032_FPS_Q=660-MT9V032_FPS_A);
		MT9V032_FPS_V=(short)((((double)MT9V032_PCLK/fps)-4)/(MT9V032_FPS_A+MT9V032_FPS_Q)-MT9V032_FPS_N);
		if((short)MT9V032_FPS_V<4) MT9V032_FPS_V=4;
	}
	if(MT_IIC_WR_REG(MT9V032_REG_CFG[4][0],MT9V032_REG_CFG[4][1]))
		s--;
	if(MT_IIC_WR_REG(MT9V032_REG_CFG[5][0],MT9V032_REG_CFG[5][1]))
		s--;
	return s;
}

///**
//   @brief      MT9V032DCMI的DMA传输完成中断函数
//   @param      NULL
//   @return     void
//   @since      v1.0
//*/
//void DMA2_Stream1_IRQHandler(void){
//	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)!=RESET){
//		int i,j;
//		
//		for(i=0;i<MT9V032_HEIGHT;i++){
//			LCD_SetCursor(0,i);
//			LCD->LCD_REG=lcddev.wramcmd;
//			for(j=0;j<MT9V032_WIDTH;j++){
//				LCD->LCD_RAM=MONO_RGB565[pic_data[i*MT9V032_WIDTH+j]];
//			}
//		}
//		fps_count++;
//		POINT_COLOR=RED;
//		LCD_ShowxNum(216,0,fps,3,16,1);
//		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
//		DMA2_Stream1->NDTR=MT9V032_WIDTH*MT9V032_HEIGHT/4;
//		DMA_Cmd(DMA2_Stream1,ENABLE);
//		DCMI_CaptureCmd(ENABLE);
//		
//	}
//}

///**
//   @brief      TIM3初始化函数
//   @param      NULL
//   @return     void
//   @since      v1.0
//*/
//void TIM3_Init(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	
//	TIM_TimeBaseInitStructure.TIM_Prescaler=8399;
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_Period=5000;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM3,ENABLE); 
//}

///**
//   @brief      TIM3中断函数
//   @param      NULL
//   @return     void
//   @since      v1.0
//*/
//void TIM3_IRQHandler(void)
//{
//	static u8 l=0;
//	l++;
//	if(l>=4) l=0;
//	if(l<2) LED2_ON
//	else if(l<4) LED2_OFF
//	
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET) //溢出中断
//	{
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
//		fps=fps_count*2;
//		fps_count=0;
//	}
//	
//}

