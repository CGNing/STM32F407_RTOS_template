/************************GUO NING*******************
���ߣ�CGN
�ļ�����key.c
�������ڣ�2018.3.11
ά���ˣ�
ά�����ڣ�
������
****************************************************/
#include "key.h"

void KEY_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	 
}

u8 KEY_read(void){
	return KEY0+KEY1*2+KEY2*4;
}
