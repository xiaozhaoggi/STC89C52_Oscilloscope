#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器
#include "lcd.h"
#include "stdio.h"
#include "adc0809.h"
#include "Keyscan.h"
#include <string.h>

#define DATAMAX 127				//采样数量

bit flags = 0;					//中断定时标志
uint32 freq = 5000;				//单片机采样频率（此频率低于ADC采样频率）
uint8 amp;						//限幅系数
uint8 idata addata[DATAMAX];	//AD值缓冲区
uint8 TH_0, TL_0;				//T0定时器初值（在中断服务函数外计算，减少中断服务函数对采样频率的影响）
uint8 modec = 0;				//显示模式，1连续，2单次
bit nodelay = 0;				//波形连续无延时显示，0有延时，1无延时

unsigned char code vaxis[];
unsigned char code haxis[];
void T0Init();					//T0定时器配置函数
void ShowWaveform();			//波形显示函数
uint8 LongToString(uint8 *str, int32 dat);		//长整型转换成字符串函数
void KeySet(uint8 *keyValue);					//按键处理函数
void Display(void);								//采样显示函数

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/


void main()
{
	uint8 keyValue;
	T0Init();									//配置定时器T0
  EA = 1;		
  InitLcd12864();
	ADCInit();			//初始化ADC0809
	
	LcdShowString(16, 16, "简易示波器");
	Delay10Ms(50);
	Delay10Ms(50);
	Delay10Ms(50);
	LcdWriteCmd(0x30);							//因上面切换到了图形模式，因此要清零必须切回基本模式
	LcdWriteCmd(0x01);							//清零字符显示（基本指令）
	LcdWriteChar57(0, 0, "K1 CHECK NO DELAY");
	LcdWriteChar57(0, 10, "K2 CHECK 50MS DELAY");
	LcdWriteChar57(0, 20, "K3 FREQ+");
	LcdWriteChar57(0, 30, "K4 FREQ-");
	LcdWriteChar57(7,40, "200HZ < FREQ < 5KHZ");
	LcdWriteChar57(25,55, "Press any key");
	
	while(1)
	{
	
		Key_Scan(&keyValue);
		if(keyValue > 0)
			KeySet(&keyValue);
		if(modec == 1)
			Display();

	}		
}


/* 采样显示函数 */
void Display(void)
{
	uint8 i = 0;
	while(i < DATAMAX)
		if (flags)								//执行采样
		{
			flags = 0;
			ADC(addata + i);					//启动AD转换
			i++;
		}
	ShowWaveform();								//显示波形
}

/* 按键处理函数 */
void KeySet(uint8 *keyValue)
{
	if(*keyValue == 0x10)
	{
		modec = 1;
		nodelay = 1;
	}
	if(*keyValue == 0x20)
	{
		modec = 1;
		nodelay = 0;
	}
	
	if(*keyValue == 0x30 && freq < 50000)
		freq += 200;
	
	if(*keyValue == 0x40 && freq > 200)
		freq -= 200;
	
	*keyValue = 0;
	if(modec == 0)
		modec = 2;
	
	TH_0 = 65536-SYS_MCLK/freq >> 8;			//重新计算T0定时器初值
	TL_0 = 65536-SYS_MCLK/freq;
}

/* 波形显示函数 */
void ShowWaveform()
{
	uint8 i=0, tmp=0, n1=0, n2=0, n3=0, len=0;
	uint8 str[25], str1[6];				//波形参数显示字符串缓冲区
	uint32 wf, Vmax=0, Vmin=255;		//频率，电压最大值，电压最小值
	int8 updown;						//波形垂直方向自动调整量
	
	/* 查询波峰、波谷，即电压最大值和最小值 */
	for(i = 0; i < DATAMAX; i++)
	{
		if(Vmax < addata[i])
			Vmax = addata[i];			//查询电压最大值
		if(Vmin > addata[i])
			Vmin = addata[i];			//查询电压最小值
	}
	
	/* 去除直流部分 */
	for(i = 0; i < DATAMAX; i++)
		addata[i] -= Vmin;
	
	/* 计算限幅系数 */
	tmp = Vmax - Vmin;
	if(tmp > 200)
		amp = 5;
	else if(tmp > 150 && tmp <= 200)
		amp = 4;
	else if(tmp > 100 && tmp <=150)
		amp = 3;
	else if(tmp > 50 && tmp <= 100)
		amp = 2;
	else
		amp = 1;
	
	tmp = 0;
	for(i = 0; i < DATAMAX; i++)
		if(tmp < addata[i]/amp)
		{
			tmp = addata[i]/amp;		//查询第一个电压最大值
			n1 = i;
		}
	for(i = n1; i < DATAMAX; i++)
		if(tmp > addata[i]/amp)
		{
			tmp = addata[i]/amp;		//查询第一个电压最小值
			n2 = i;
		}
	for(i = n2; i < DATAMAX; i++)
		if(tmp < addata[i]/amp)
		{
			tmp = addata[i]/amp;		//查询第二个电压最大值
			n3 = i;
		}

	/* 根据波谷、波峰计算垂直方向居中的调整量 */
	updown = 25 - (Vmax - Vmin)/amp/2;	//垂直方向居中的调整量
	
	/* 计算波形参数 */
	wf = freq/(n3 - n1);				//计算频率（单位：Hz）
	Vmax = Vmax*1000/51;				//计算电压最大值（单位：mV）
	Vmin = Vmin*1000/51;				//计算电压最小值（单位：mV）

	/* 显示坐标轴 */
	LcdClearArea(0, 0, 128, 64);		//清屏（从这儿开始图形模式指令）
	LcdShowImage(0, 0,  16, 51, vaxis);	//画纵轴
	LcdShowImage(0, 51, 128, 4, haxis);	//画横轴
	LcdWriteChar57(5, 0, "v");
	LcdWriteChar57(121, 43, "t");
	
	/* 显示波形 */
	for(i = 0; i < 127; i++)
		LcdShowPoint(i+1, 51 - addata[i]/amp - updown, 1);

	/* 显示波形参数 */
	str[0] = '\0';
	strcat(str, "SF=");
	len = LongToString(str1, freq);		//采样频率
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "Hz");
	LcdWriteChar57(0, 56, str);
	if(!nodelay)
		Delay10Ms(5);
	
	len = LongToString(str, wf);		//信号频率
	str[len] = '\0';
	strcat(str, "Hz ");
	len = LongToString(str1, Vmin);		//最小电压
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "mv~");
	len = LongToString(str1, Vmax);		//最大电压
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "mv");
	LcdWriteChar57(0, 56, str);
	if(!nodelay)
		Delay10Ms(10);
}

/* 长整型数转换为字符串，str-字符串指针，dat-待转换数，返回值-字符串长度 */
uint8 LongToString(uint8 *str, int32 dat)
{
    int8 i = 0;
    uint8 len = 0;
    uint8 buf[12];
    
    if (dat < 0)		//如果为负数，首先取绝对值，并在指针上添加负号
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do {				//先转换为低位在前的十进制数组
        buf[i++] = dat % 10;
        dat /= 10;
    } while (dat > 0);
    len += i;			//i最后的值就是有效字符的个数
    while (i-- > 0)		//将数组值转换为ASCII码反向拷贝到接收指针上
    {
        *str++ = buf[i] + '0';
    }
    *str = '\0';		//添加字符串结束符
    
    return len;			//返回字符串长度
}

/* T0定时器初始化 */
void T0Init()
{
    TMOD &= 0xF0;		//清零T0的控制位
    TMOD |= 0x01;		//配置T0为模式1
	TH_0 = 65536-SYS_MCLK/freq >> 8;	//除数即为中断频率
	TL_0 = 65536-SYS_MCLK/freq;
    TH0   = TH_0;		//设定T0初值
    TL0   = TL_0;
	ET0   = 1;			//使能T0中断
	TR0   = 1;			//启动T0
}

/* T0定时器中断服务函数 */
void T0_ISR() interrupt 1
{
    //T0重新赋初值
    TH0 = TH_0;
    TL0 = TL_0;
    flags = 1;
}

/* 示波器纵轴 */
const unsigned char code vaxis[] = {
	0x40,0x00,
	0xE0,0x00,
	0x40,0x00,
	0x70,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x60,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x70,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x60,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x70,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x60,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x70,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x60,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x70,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x40,0x00,
	0x60,0x00,
	0x40,0x00,
	0x40,0x00,
};

/* 示波器横轴 */
const unsigned char code haxis[] = {
	0x40,0x10,0x04,0x01,0x00,0x40,0x10,0x04,0x01,0x00,0x40,0x10,0x04,0x01,0x00,0x40,
	0x42,0x10,0x84,0x21,0x08,0x42,0x10,0x84,0x21,0x08,0x42,0x10,0x84,0x21,0x08,0x42,
	0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
};