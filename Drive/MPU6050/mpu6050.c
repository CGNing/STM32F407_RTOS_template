/************************GUO NING*******************
作者：CGN
文件名：mpu6050.c
创建日期：2017.9.11
维护人：
维护日期：
描述：mpu6050驱动函数
管脚：
****************************************************/

#include "mpu6050.h"


struct _MPU6050 MPU6050;
u8 mpu6050_ok;

void I2cwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data);
void I2cwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
void MPU6050_INT_Init(void);

void MPU6050_Init(u16 lpf){
	u16 default_filter = 1;
	
	switch(lpf)
    {
    case 5:
        default_filter = MPU6050_DLPF_BW_5;
        break;
    case 10:
        default_filter = MPU6050_DLPF_BW_10;
        break;
    case 20:
        default_filter = MPU6050_DLPF_BW_20;
        break;
    case 42:
        default_filter = MPU6050_DLPF_BW_42;
        break;
    case 98:
        default_filter = MPU6050_DLPF_BW_98;
        break;
    case 188:
        default_filter = MPU6050_DLPF_BW_188;
        break;
    case 256:
        default_filter = MPU6050_DLPF_BW_256;
        break;
    default:
        default_filter = MPU6050_DLPF_BW_42;
        break;
    }
		
	MPU6050_INT_Init();
	I2c_Soft_Init();
	
//	I2cwriteBit(MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, 1);//复位
//	delay_ms(10);
	I2cwriteBit(MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);//唤醒MPU6050
	delay_ms(10);
	I2c_Write_1Byte(MPU6050_ADDR,MPU6050_REG_INT_ENABLE,0X00);//关闭所有中断
	delay_ms(1);
	I2c_Write_1Byte(MPU6050_ADDR,MPU6050_REG_INT_PIN_CFG,0X80);//INT引脚低电平有效
	delay_ms(1);
	I2c_Write_1Byte(MPU6050_ADDR,MPU6050_REG_PWR_MGMT_2,0X00);//加速度与陀螺仪都工作
	delay_ms(1);
	I2c_Write_1Byte(MPU6050_ADDR,MPU6050_REG_FIFO_EN,0X00);//关闭FIFO
	delay_ms(1);
	I2cwriteBits(MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH,MPU6050_CLOCK_PLL_ZGYRO);//设置时钟
	delay_ms(1);
	I2c_Write_1Byte(MPU6050_ADDR, MPU6050_REG_SMPLRT_DIV,0);//采样率1000HZ
	delay_ms(1);
	I2cwriteBits(MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS_2000);//陀螺仪最大量程+-2000度每秒
  I2cwriteBits(MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG,7, 3, 0x00);   //不自检
	delay_ms(1);
	I2cwriteBits(MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS_8);//加速度最大量程+-8G
  I2cwriteBits(MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG,7, 3, 0x00);  //不自检
	delay_ms(1);
	I2cwriteBits(MPU6050_ADDR, MPU6050_REG_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, default_filter);//42Hz
	delay_ms(1);
	I2cwriteBit(MPU6050_ADDR, MPU6050_REG_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, 0);//不让MPU6050 控制AUXI2C
	delay_ms(1);
	I2cwriteBit(MPU6050_ADDR, MPU6050_REG_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 1);//主控制器的I2C与MPU6050的AUXI2C直通
	delay_ms(1);
}

void I2cwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
    u8 b;
    I2c_Read_nByte(dev, reg, 1, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    mpu6050_ok = !( I2c_Write_1Byte(dev, reg, b) );
}

void I2cwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data){

    u8 b,mask;
    I2c_Read_nByte(dev, reg, 1, &b);
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    I2c_Write_1Byte(dev, reg, b);
}

void MPU6050_INT_Init(void){
  //	GPIO_InitTypeDef  GPIO_InitStructure; 
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//	
	//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//	
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	//	
	//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void MPU6050_Read(void){
	I2C_FastMode = 1;
//	I2c_Read_nByte(MPU6050_ADDR,MPU6050_REG_ACCEL_XOUT_H,14,&((u8)MPU6050.AX));
}

short MPU_Get_Temperature(void){
	u8 buf[2]; 
  short raw;
	float temp;
	I2c_Read_nByte(MPU6050_ADDR,MPU6050_REG_TEMP_OUT_H,2,buf); 
  raw=((u16)buf[0]<<8)|buf[1];  
  temp=36.53+((double)raw)/340;  
  return temp*100;
}

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz){
	u8 buf[6],res;  
	res=I2c_Read_nByte(MPU6050_ADDR,MPU6050_REG_GYRO_XOUT_H,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}

u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az){
	u8 buf[6],res;  
	res=I2c_Read_nByte(MPU6050_ADDR,MPU6050_REG_ACCEL_XOUT_H,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
