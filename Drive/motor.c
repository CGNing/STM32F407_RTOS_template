/************************GUO NING*******************
作者：CGN
文件名：motor.c
创建日期：2017.7.23
维护人：
维护日期：
描述：驱动控制程序
管脚：ENA:PWM1,PB8,TIM4_CH3
			A1:B10
			A2:B11
			ENB:PWM2,PB9,TIM4_CH4
			B1:B12
			B2:B13
****************************************************/

#include "motor.h"

void motor_Init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period=1000;
	TIM_TimeBaseStructure.TIM_Prescaler=0;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE);
	
}

void move(int L,int R){
	if(L>0){
		//左前进
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
	}
	else if(L<0){
		//左后退
		L=-L;
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
	else{
		//左制动
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
	
	if(R>0){
		//右前进
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
	}
	else if(R<0){
		//右后退
		R=-R;
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	else{
		//右制动
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	
	TIM_SetCompare3(TIM4,L);
	TIM_SetCompare4(TIM4,R);
}



