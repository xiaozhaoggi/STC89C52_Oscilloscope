#include "adc0809.h"

/********ת������********/
void ADC(unsigned char *ad)
{
	/*STR=1;							//STR��������ADת��
	STR=0;*/
	START = 1;//��������ADת��
	ALE = 1;
	START = 0;
	ALE = 0;
	while(EOC==0);					//�ȴ�ת������
	OE=1;
	*ad = ADC0809_DATA;				//��ȡAD���ݣ����ݷ�ΧΪ0-255
	OE=0;
}

/*********T1��ʱ����ʼ��*********/
void T1Init()
{
    TMOD &= 0x0F;					//����T1�Ŀ���λ
    TMOD |= 0x20;					//����T1Ϊģʽ2(8λ�Զ���װģʽ)
	TH1 = (65536-46080)/256;
	TL1 = (65536-46080)%256;
	ET1  = 1;						//����ʱ��1�ж�
	TR1  = 1;						//������ʱ��T1
}

/*********ADC0809��ʼ��*********/
void ADCInit()
{
	T1Init();
	OE=0;
	//STR=0;
	START = 0;
	ALE = 0;
}

/************T1�жϷ������************/
void T1_ISR() interrupt 3 using 0
{
	CLK = ~CLK;						//����T1�жϲ���CLK�ź�
}

void Delay10Ms(unsigned int Ms)

{
unsigned char i,j,k;
for(;Ms>0;Ms--)
	
for(i=5;i>0;i--)

for(j=4;j>0;j--)

for(k=248;k>0;k--);

}