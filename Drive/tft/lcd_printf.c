#include "stm32f4xx.h"
#include "lcd.h"

#include <stdio.h>

#define PRINT_SIZE 16
#define PRINT_POINTCOLOR WHITE
#define PRINT_BACKCOLOR BLACK

int fputc(int ch,FILE *p) 
{
	static int x=0,y=0;
	
	//page turing
	if(x==0&&y==0)
		LCD_Clear(PRINT_BACKCOLOR);
	
	if((ch<='~')&&(ch>=' ')){
		POINT_COLOR=PRINT_POINTCOLOR;
		LCD_ShowChar(x,y,ch,PRINT_SIZE,1);
		x+=PRINT_SIZE/2;
		
		//new line while can not print a full char
		if(x>=(lcddev.width-PRINT_SIZE/2)){
		x=0;
		y+=PRINT_SIZE;
		}
	}
	else if(ch=='\n'){
		x=0;
		y+=PRINT_SIZE;
	}
	
	//new page while can not print a full line
	if(y>=(lcddev.height-PRINT_SIZE)){
		y=0;
	}
	
	return ch;
}
