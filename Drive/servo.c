/************************GUO NING*******************
作者：CGN
文件名：servo.c
创建日期：2018.5.2
维护人：舵机控制程序
维护日期：
描述：
管脚：S1:E5 TIM9 CH1
			S2:E6 TIM9 CH2
			S3:F6 TIM10 CH1
****************************************************/
#include "servo.h"

void servo_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period=20000;
	TIM_TimeBaseStructure.TIM_Prescaler=167;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);
	TIM_OC2Init(TIM9,&TIM_OCInitStructure);
	TIM_OC1Init(TIM10,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM9,ENABLE);
	TIM_Cmd(TIM10,ENABLE);
}

void servo_angle(u8 id,u8 angle){
	u16 time;
	if(angle>180)
		angle=180;
	time=2000*angle/180+500;
	switch(id){
		case 0:
			TIM_SetCompare1(TIM9,time);
			break;
		case 1:
			TIM_SetCompare2(TIM9,time);
			break;
		case 2:
			TIM_SetCompare1(TIM10,time);
			break;
	}
}
void servo_time(u8 id,u16 time){
	switch(id){
		case 0:
			TIM_SetCompare1(TIM9,time);
			break;
		case 1:
			TIM_SetCompare2(TIM9,time);
			break;
		case 2:
			TIM_SetCompare1(TIM10,time);
			break;
	}
}

void servo_test(u8 id){
	u16 i;
	for(i=500;i<=2500;i++){
		servo_time(id,i);
		delay_ms(2);
	}
	for(i=2500;i>=500;i--){
		servo_time(id,i);
		delay_ms(2);
	}
}
