#include "reg52.h"    //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "lcd.h"
#include "stdio.h"
#include "adc0809.h"
#include "Keyscan.h"
#include <string.h>

#define DATAMAX 127				//��������

bit flags = 0;					//�ж϶�ʱ��־
uint32 freq = 5000;				//��Ƭ������Ƶ�ʣ���Ƶ�ʵ���ADC����Ƶ�ʣ�
uint8 amp;						//�޷�ϵ��
uint8 idata addata[DATAMAX];	//ADֵ������
uint8 TH_0, TL_0;				//T0��ʱ����ֵ�����жϷ���������㣬�����жϷ������Բ���Ƶ�ʵ�Ӱ�죩
uint8 modec = 0;				//��ʾģʽ��1������2����
bit nodelay = 0;				//������������ʱ��ʾ��0����ʱ��1����ʱ

unsigned char code vaxis[];
unsigned char code haxis[];
void T0Init();					//T0��ʱ�����ú���
void ShowWaveform();			//������ʾ����
uint8 LongToString(uint8 *str, int32 dat);		//������ת�����ַ�������
void KeySet(uint8 *keyValue);					//����������
void Display(void);								//������ʾ����

/*******************************************************************************
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/


void main()
{
	uint8 keyValue;
	T0Init();									//���ö�ʱ��T0
  EA = 1;		
  InitLcd12864();
	ADCInit();			//��ʼ��ADC0809
	
	LcdShowString(16, 16, "����ʾ����");
	Delay10Ms(50);
	Delay10Ms(50);
	Delay10Ms(50);
	LcdWriteCmd(0x30);							//�������л�����ͼ��ģʽ�����Ҫ��������лػ���ģʽ
	LcdWriteCmd(0x01);							//�����ַ���ʾ������ָ�
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


/* ������ʾ���� */
void Display(void)
{
	uint8 i = 0;
	while(i < DATAMAX)
		if (flags)								//ִ�в���
		{
			flags = 0;
			ADC(addata + i);					//����ADת��
			i++;
		}
	ShowWaveform();								//��ʾ����
}

/* ���������� */
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
	
	TH_0 = 65536-SYS_MCLK/freq >> 8;			//���¼���T0��ʱ����ֵ
	TL_0 = 65536-SYS_MCLK/freq;
}

/* ������ʾ���� */
void ShowWaveform()
{
	uint8 i=0, tmp=0, n1=0, n2=0, n3=0, len=0;
	uint8 str[25], str1[6];				//���β�����ʾ�ַ���������
	uint32 wf, Vmax=0, Vmin=255;		//Ƶ�ʣ���ѹ���ֵ����ѹ��Сֵ
	int8 updown;						//���δ�ֱ�����Զ�������
	
	/* ��ѯ���塢���ȣ�����ѹ���ֵ����Сֵ */
	for(i = 0; i < DATAMAX; i++)
	{
		if(Vmax < addata[i])
			Vmax = addata[i];			//��ѯ��ѹ���ֵ
		if(Vmin > addata[i])
			Vmin = addata[i];			//��ѯ��ѹ��Сֵ
	}
	
	/* ȥ��ֱ������ */
	for(i = 0; i < DATAMAX; i++)
		addata[i] -= Vmin;
	
	/* �����޷�ϵ�� */
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
			tmp = addata[i]/amp;		//��ѯ��һ����ѹ���ֵ
			n1 = i;
		}
	for(i = n1; i < DATAMAX; i++)
		if(tmp > addata[i]/amp)
		{
			tmp = addata[i]/amp;		//��ѯ��һ����ѹ��Сֵ
			n2 = i;
		}
	for(i = n2; i < DATAMAX; i++)
		if(tmp < addata[i]/amp)
		{
			tmp = addata[i]/amp;		//��ѯ�ڶ�����ѹ���ֵ
			n3 = i;
		}

	/* ���ݲ��ȡ�������㴹ֱ������еĵ����� */
	updown = 25 - (Vmax - Vmin)/amp/2;	//��ֱ������еĵ�����
	
	/* ���㲨�β��� */
	wf = freq/(n3 - n1);				//����Ƶ�ʣ���λ��Hz��
	Vmax = Vmax*1000/51;				//�����ѹ���ֵ����λ��mV��
	Vmin = Vmin*1000/51;				//�����ѹ��Сֵ����λ��mV��

	/* ��ʾ������ */
	LcdClearArea(0, 0, 128, 64);		//�������������ʼͼ��ģʽָ�
	LcdShowImage(0, 0,  16, 51, vaxis);	//������
	LcdShowImage(0, 51, 128, 4, haxis);	//������
	LcdWriteChar57(5, 0, "v");
	LcdWriteChar57(121, 43, "t");
	
	/* ��ʾ���� */
	for(i = 0; i < 127; i++)
		LcdShowPoint(i+1, 51 - addata[i]/amp - updown, 1);

	/* ��ʾ���β��� */
	str[0] = '\0';
	strcat(str, "SF=");
	len = LongToString(str1, freq);		//����Ƶ��
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "Hz");
	LcdWriteChar57(0, 56, str);
	if(!nodelay)
		Delay10Ms(5);
	
	len = LongToString(str, wf);		//�ź�Ƶ��
	str[len] = '\0';
	strcat(str, "Hz ");
	len = LongToString(str1, Vmin);		//��С��ѹ
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "mv~");
	len = LongToString(str1, Vmax);		//����ѹ
	str1[len] = '\0';
	strcat(str, str1);
	strcat(str, "mv");
	LcdWriteChar57(0, 56, str);
	if(!nodelay)
		Delay10Ms(10);
}

/* ��������ת��Ϊ�ַ�����str-�ַ���ָ�룬dat-��ת����������ֵ-�ַ������� */
uint8 LongToString(uint8 *str, int32 dat)
{
    int8 i = 0;
    uint8 len = 0;
    uint8 buf[12];
    
    if (dat < 0)		//���Ϊ����������ȡ����ֵ������ָ������Ӹ���
    {
        dat = -dat;
        *str++ = '-';
        len++;
    }
    do {				//��ת��Ϊ��λ��ǰ��ʮ��������
        buf[i++] = dat % 10;
        dat /= 10;
    } while (dat > 0);
    len += i;			//i����ֵ������Ч�ַ��ĸ���
    while (i-- > 0)		//������ֵת��ΪASCII�뷴�򿽱�������ָ����
    {
        *str++ = buf[i] + '0';
    }
    *str = '\0';		//����ַ���������
    
    return len;			//�����ַ�������
}

/* T0��ʱ����ʼ�� */
void T0Init()
{
    TMOD &= 0xF0;		//����T0�Ŀ���λ
    TMOD |= 0x01;		//����T0Ϊģʽ1
	TH_0 = 65536-SYS_MCLK/freq >> 8;	//������Ϊ�ж�Ƶ��
	TL_0 = 65536-SYS_MCLK/freq;
    TH0   = TH_0;		//�趨T0��ֵ
    TL0   = TL_0;
	ET0   = 1;			//ʹ��T0�ж�
	TR0   = 1;			//����T0
}

/* T0��ʱ���жϷ����� */
void T0_ISR() interrupt 1
{
    //T0���¸���ֵ
    TH0 = TH_0;
    TL0 = TL_0;
    flags = 1;
}

/* ʾ�������� */
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

/* ʾ�������� */
const unsigned char code haxis[] = {
	0x40,0x10,0x04,0x01,0x00,0x40,0x10,0x04,0x01,0x00,0x40,0x10,0x04,0x01,0x00,0x40,
	0x42,0x10,0x84,0x21,0x08,0x42,0x10,0x84,0x21,0x08,0x42,0x10,0x84,0x21,0x08,0x42,
	0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
};