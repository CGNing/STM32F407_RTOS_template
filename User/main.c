#include <stdio.h>

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
//#include "queue.h"

#include "userdef.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "lcd.h"


void previous_conf(void);
void task1(void *para);
void task2(void *para);

/**
   @brief      主函数
   @param      NULL
   @return     int
   @since      v1.0
*/
int main(void)
{
	previous_conf();
	xTaskCreate(task1,"task1",configMINIMAL_STACK_SIZE,NULL,3,NULL);
	xTaskCreate(task2,"task2",configMINIMAL_STACK_SIZE,NULL,3,NULL);
	vTaskStartScheduler();
	return 0;
}

/**
   @brief      全部初始化
   @param      NULL
   @return     void
   @since      v1.0
*/
void previous_conf(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_conf(168000000);
	USART1_Init(115200);
}

void task1(void *a){
	LCD_Init();
	LCD_Clear(BLACK);
	printf("LCD init OK\n");
	while(1){
		printf("this is a try\n");
		vTaskDelay(1000);
	}
}

void task2(void *para){
	LED_Init();
	KEY_Init();
	while(1){
		LED1_ON;
		delay_ms(1000);
		LED1_OFF;
		delay_ms(1000);
	}
}
