#include "adc0809.h"

/********转换函数********/
void ADC(unsigned char *ad)
{
	/*STR=1;							//STR脉冲启动AD转换
	STR=0;*/
	START = 1;//脉冲启动AD转换
	ALE = 1;
	START = 0;
	ALE = 0;
	while(EOC==0);					//等待转换结束
	OE=1;
	*ad = ADC0809_DATA;				//读取AD数据，数据范围为0-255
	OE=0;
}

/*********T1定时器初始化*********/
void T1Init()
{
    TMOD &= 0x0F;					//清零T1的控制位
    TMOD |= 0x20;					//配置T1为模式2(8位自动重装模式)
	TH1 = (65536-46080)/256;
	TL1 = (65536-46080)%256;
	ET1  = 1;						//开定时器1中断
	TR1  = 1;						//启动定时器T1
}

/*********ADC0809初始化*********/
void ADCInit()
{
	T1Init();
	OE=0;
	//STR=0;
	START = 0;
	ALE = 0;
}

/************T1中断服务程序************/
void T1_ISR() interrupt 3 using 0
{
	CLK = ~CLK;						//利用T1中断产生CLK信号
}

void Delay10Ms(unsigned int Ms)

{
unsigned char i,j,k;
for(;Ms>0;Ms--)
	
for(i=5;i>0;i--)

for(j=4;j>0;j--)

for(k=248;k>0;k--);

}