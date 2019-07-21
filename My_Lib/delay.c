/************************GUO NING*******************
���ߣ�CGN
�ļ�����delay.c
�������ڣ�2019.7.20
ά���ˣ�
ά�����ڣ�
��������ʱ����
****************************************************/
#include "userdef.h"
#include "delay.h"

#if( USE_FREE_RTOS==1 )
	#include "FreeRTOS.h"
	#include "task.h"
#endif

#if( USE_FREE_RTOS==1 )

static u16 count_us=0;
static u32 overflow_us=0X00FFFFFF;

static u16 is_1ms=0;


void delay_conf(u32 SYSCLK){
	count_us=SYSCLK/1000000;

	overflow_us=0X00FFFFFF/count_us;
	
	is_1ms=((1000.0/configTICK_RATE_HZ)==1.0)?1:0;

}

void delay_ms(u32 t){
	if(is_1ms==1)
		vTaskDelay(t);
	else
		vTaskDelay((t*1000)/configTICK_RATE_HZ);
}

#else

static u16 count_ms=0;
static u16 count_us=0;
static u32 overflow_ms=0X00FFFFFF;
static u32 overflow_us=0X00FFFFFF;


void delay_conf(u32 SYSCLK){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		//ѡ���ⲿʱ��
	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;		//�ر��ж�

	count_us=(SYSCLK/1000000)/8;		//����ü���ֵ
	count_ms=(SYSCLK/1000)/8;
	
	overflow_ms=0X00FFFFFF/count_ms;
	overflow_us=0X00FFFFFF/count_us;
}

void delay_ms(u32 t){
	u32 temp=0;
	u32 SysTickBak[3];		//SysTick״̬����
	
	if(t>overflow_ms){
		t=overflow_ms;
	}

	//��ʱ��ָ���ʱ��״̬
	SysTickBak[0]=SysTick->CTRL;
	SysTickBak[1]=SysTick->LOAD;
	SysTickBak[2]=SysTick->VAL;
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//��ֹ�ж���ʱ��ɳ�ͻ
	SysTick->LOAD=(u32)fac_ms*t;//1~0X00FFFFFF
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//�򿪼�ʱ��
	
	do{
		temp=SysTick->CTRL;
	}while(!(temp&SysTick_CTRL_COUNTFLAG_Msk)&&(temp&SysTick_CTRL_ENABLE_Msk));

	SysTick->CTRL=SysTickBak[0];
	SysTick->LOAD=SysTickBak[1];
	SysTick->VAL=SysTickBak[2];
}

#endif

void delay_us(u32 t){
	u32 temp=0;
	u32 SysTickBak[3];		//SysTick״̬����
	
	if(t>overflow_us){
		t=overflow_us;
	}
	
	//��ʱ��ָ���ʱ��״̬
	SysTickBak[0]=SysTick->CTRL;
	SysTickBak[1]=SysTick->LOAD;
	SysTickBak[2]=SysTick->VAL;
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;		//��ֹ�ж���ʱ��ɳ�ͻ
	SysTick->LOAD=(u32)count_us*t;		//1~0X00FFFFFF
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;		//�򿪼�ʱ��
	
	do{
		temp=SysTick->CTRL;
	}while(!(temp&SysTick_CTRL_COUNTFLAG_Msk)&&temp&SysTick_CTRL_ENABLE_Msk);

	SysTick->CTRL=SysTickBak[0];
	SysTick->LOAD=SysTickBak[1];
	SysTick->VAL=SysTickBak[2];
}
