#ifndef _ADC0809_H_
#define _ADC0809_H_

#include "reg52.h" 

/* 全局运行参数定义 */
#define OSC_FREQ 11059200		//晶振频率值，单位Hz
#define SYS_MCLK (OSC_FREQ/12)	//定时器频率(STC89和12系列的定时器默认都是振荡器频率÷12）

/* 数据类型定义 */
typedef signed   char int8;		// 8位有符号整型数
typedef signed   int  int16;	//16位有符号整型数
typedef signed   long int32;	//32位有符号整型数
typedef unsigned char uint8;	// 8位无符号整型数
typedef unsigned int  uint16;	//16位无符号整型数
typedef unsigned long uint32;	//32位无符号整型数


//ADC0809模块接线
#define ADC0809_DATA P1		//ADC0809模块的数据引脚D0-D7
//sbit STR 		= P3^7;			//STR引脚，启动转换信号
sbit START	= P2^0;			//STR引脚，启动转换信号
sbit ALE 		= P2^1;			//STR引脚，启动转换信号
sbit CLK 		= P2^2;			//CLK，ADC0809时钟输入50～800KHZ的频率，一般选用500KHZ
sbit OE  		= P2^3;			//OE，输出允许信号，高电平有效
sbit EOC 		= P2^4;			//EOC，转换结束信号，高电平有效



/********************************************
地址选择 A=1;B=1;C=0
选择通道IN3，当然也可以通过软件设置地址
	REF+ 接VCC
	REF- 接GND
这样 AD=255*(VIN-(VREF-))/((VREF+)-(VREF-))
	(VREF+) = 5V;
	(VREF-) = 0V;
	AD = 255*VIN/5
	所以IN3口的电压 VIN = AD*5/255 = AD/51;
********************************************/

void ADC(unsigned char *ad);//转换函数
void ADCInit();				//ADC0809初始化
void Delay10Ms(unsigned int Ms);
#endif
