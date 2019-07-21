#include "key.h"

void KEY_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//Ê¹ÄÜGPIOA,GPIOEÊ±ÖÓ
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 KEY_read(void){
	return KEY0+KEY1*2+KEY_WK_UP*4;
}
