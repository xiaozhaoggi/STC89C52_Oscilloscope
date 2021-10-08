#ifndef __LCD_H
#define __LCD_H

#include "reg52.h" 
#include <intrins.h>


//---�ض���ؼ���---//
#ifndef Uint
#define Uint unsigned int
#endif

#ifndef Uchar 
#define Uchar unsigned char
#endif

//--��ʱʹ�õ�IO��--//
#define LCD12864_DB P0
sbit LCD_RST = P3^4;
sbit LCD12864_RS   = P3^5;
sbit LCD12864_RW   = P3^6;
sbit LCD12864_E	  = P3^7;

static void Wait1ms(void);//�ӳ�1 ms
void WaitNms(int n);
void LcdWaitReady();																	//12864Һ��æ״̬��⺯��
void LcdWriteCmd(unsigned char cmd);													//��Һ��д��һ�ֽ����cmd-��д������ֵ
void LcdWriteDat(unsigned char dat);													//��Һ��д��һ�ֽ����ݣ�dat-��д������ֵ
unsigned char LcdReadDat();																//��Һ������һ�ֽ�����
void LcdShowString(unsigned char x, unsigned char y, unsigned char *str);				//�ַ�����ʾ����
void LcdShowImage(unsigned char x, unsigned char y, unsigned char w, unsigned char h, unsigned char *img);//ͼ����ʾ����
void LcdShowPoint(unsigned char x, unsigned char y, bit sn);							//����Ļָ�����껭��
unsigned char LcdWriteChar57(unsigned char x, unsigned char y, unsigned char *cn);		//5X7�ַ�����ʾ����
void LcdClearArea(unsigned char x, unsigned char y, unsigned char w, unsigned char h);	//�����Ļ�ϵ�һ��ͼ����ʾ����
void InitLcd12864();																	//12864Һ����ʼ������
unsigned char LcdWriteChar57(unsigned char x, unsigned char y, unsigned char *cn);
//#ifdef _CHARCODE_

/* ------------------- 5*7 ASCII��ģ�����ݽṹ���� ------------------- */
struct Char57TypeDef			// ASCII��ģ���ݽṹ
{
	unsigned char Index[1];		// �ַ���������,һ��ASCII���ַ�ռһ���ֽ�	
	unsigned char Msk[8];		// ����������(5X7��8������) 
};
/* ------ 5*7 ASCII��ģ���ݱ�(ʵ��ʹ��6*8���󣬺���ȡģ��λ����) ------ */



//#endif

#endif