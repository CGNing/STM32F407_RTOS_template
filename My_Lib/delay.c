/************************GUO NING*******************
作者：CGN
文件名：delay.c
创建日期：2019.7.20
维护人：
维护日期：
描述：延时函数
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
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);		//选择外部时钟
	SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;		//关闭中断

	count_us=(SYSCLK/1000000)/8;		//计算好加载值
	count_ms=(SYSCLK/1000)/8;
	
	overflow_ms=0X00FFFFFF/count_ms;
	overflow_us=0X00FFFFFF/count_us;
}

void delay_ms(u32 t){
	u32 temp=0;
	u32 SysTickBak[3];		//SysTick状态备份
	
	if(t>overflow_ms){
		t=overflow_ms;
	}

	//延时后恢复定时器状态
	SysTickBak[0]=SysTick->CTRL;
	SysTickBak[1]=SysTick->LOAD;
	SysTickBak[2]=SysTick->VAL;
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//防止中断延时造成冲突
	SysTick->LOAD=(u32)fac_ms*t;//1~0X00FFFFFF
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;	//打开计时器
	
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
	u32 SysTickBak[3];		//SysTick状态备份
	
	if(t>overflow_us){
		t=overflow_us;
	}
	
	//延时后恢复定时器状态
	SysTickBak[0]=SysTick->CTRL;
	SysTickBak[1]=SysTick->LOAD;
	SysTickBak[2]=SysTick->VAL;
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;		//防止中断延时造成冲突
	SysTick->LOAD=(u32)count_us*t;		//1~0X00FFFFFF
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;		//打开计时器
	
	do{
		temp=SysTick->CTRL;
	}while(!(temp&SysTick_CTRL_COUNTFLAG_Msk)&&temp&SysTick_CTRL_ENABLE_Msk);

	SysTick->CTRL=SysTickBak[0];
	SysTick->LOAD=SysTickBak[1];
	SysTick->VAL=SysTickBak[2];
}
