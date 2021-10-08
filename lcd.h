#ifndef __LCD_H
#define __LCD_H

#include "reg52.h" 
#include <intrins.h>


//---重定义关键词---//
#ifndef Uint
#define Uint unsigned int
#endif

#ifndef Uchar 
#define Uchar unsigned char
#endif

//--定时使用的IO口--//
#define LCD12864_DB P0
sbit LCD_RST = P3^4;
sbit LCD12864_RS   = P3^5;
sbit LCD12864_RW   = P3^6;
sbit LCD12864_E	  = P3^7;

static void Wait1ms(void);//延迟1 ms
void WaitNms(int n);
void LcdWaitReady();																	//12864液晶忙状态检测函数
void LcdWriteCmd(unsigned char cmd);													//向液晶写入一字节命令，cmd-待写入命令值
void LcdWriteDat(unsigned char dat);													//向液晶写入一字节数据，dat-待写入数据值
unsigned char LcdReadDat();																//从液晶读出一字节数据
void LcdShowString(unsigned char x, unsigned char y, unsigned char *str);				//字符串显示函数
void LcdShowImage(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char *img);//图像显示函数
void LcdShowPoint(unsigned char x, unsigned char y, bit sn);							//在屏幕指定坐标画点
unsigned char LcdWriteChar57(unsigned char x, unsigned char y, unsigned char *cn);		//5X7字符串显示函数
void LcdClearArea(unsigned char x, unsigned char y, unsigned char w, unsigned char h);	//清除屏幕上的一块图形显示区域
void InitLcd12864();																	//12864液晶初始化函数
unsigned char LcdWriteChar57(unsigned char x, unsigned char y, unsigned char *cn);
//#ifdef _CHARCODE_

/* ------------------- 5*7 ASCII字模的数据结构定义 ------------------- */
struct Char57TypeDef			// ASCII字模数据结构
{
	unsigned char Index[1];		// 字符内码索引,一个ASCII码字符占一个字节	
	unsigned char Msk[8];		// 点阵码数据(5X7有8个数据) 
};
/* ------ 5*7 ASCII字模数据表(实际使用6*8点阵，横向取模高位在左) ------ */



//#endif

#endif