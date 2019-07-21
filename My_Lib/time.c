/************************GUO NING*******************
作者：CGN
文件名：time.c
创建日期：2018.1.6
维护人：
维护日期：
描述：TIM中断设置
****************************************************/
#include "time.h"

void TIM1_10ms_Init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;	
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);					      
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 99; 
  TIM_TimeBaseStructure.TIM_Period = 16799; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Reset counter
  TIM_SetCounter(TIM1,0);
  TIM_Cmd(TIM1, ENABLE); 
}

