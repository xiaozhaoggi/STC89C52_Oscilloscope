#ifndef _ADC0809_H_
#define _ADC0809_H_

#include "reg52.h" 

/* ȫ�����в������� */
#define OSC_FREQ 11059200		//����Ƶ��ֵ����λHz
#define SYS_MCLK (OSC_FREQ/12)	//��ʱ��Ƶ��(STC89��12ϵ�еĶ�ʱ��Ĭ�϶�������Ƶ�ʡ�12��

/* �������Ͷ��� */
typedef signed   char int8;		// 8λ�з���������
typedef signed   int  int16;	//16λ�з���������
typedef signed   long int32;	//32λ�з���������
typedef unsigned char uint8;	// 8λ�޷���������
typedef unsigned int  uint16;	//16λ�޷���������
typedef unsigned long uint32;	//32λ�޷���������


//ADC0809ģ�����
#define ADC0809_DATA P1		//ADC0809ģ�����������D0-D7
//sbit STR 		= P3^7;			//STR���ţ�����ת���ź�
sbit START	= P2^0;			//STR���ţ�����ת���ź�
sbit ALE 		= P2^1;			//STR���ţ�����ת���ź�
sbit CLK 		= P2^2;			//CLK��ADC0809ʱ������50��800KHZ��Ƶ�ʣ�һ��ѡ��500KHZ
sbit OE  		= P2^3;			//OE����������źţ��ߵ�ƽ��Ч
sbit EOC 		= P2^4;			//EOC��ת�������źţ��ߵ�ƽ��Ч



/********************************************
��ַѡ�� A=1;B=1;C=0
ѡ��ͨ��IN3����ȻҲ����ͨ��������õ�ַ
	REF+ ��VCC
	REF- ��GND
���� AD=255*(VIN-(VREF-))/((VREF+)-(VREF-))
	(VREF+) = 5V;
	(VREF-) = 0V;
	AD = 255*VIN/5
	����IN3�ڵĵ�ѹ VIN = AD*5/255 = AD/51;
********************************************/

void ADC(unsigned char *ad);//ת������
void ADCInit();				//ADC0809��ʼ��
void Delay10Ms(unsigned int Ms);
#endif
