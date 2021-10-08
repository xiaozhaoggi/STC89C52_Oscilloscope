#include "Keyscan.h"
#include "adc0809.h"

/**************°´¼üÉ¨Ãè³ÌÐò****************/
void Key_Scan(unsigned char *keyValue)
{
	unsigned char i;
	if(!KEY1)
	{
		Delay10Ms(1);
		if(!KEY1)
		{
			*keyValue = 0x10;
			i=0;
			while(i<50 && !KEY1)
			{
				Delay10Ms(1);
				i++;
			}
		}
	}
	if(!KEY2)
	{
		Delay10Ms(1);
		if(!KEY2)
		{
			*keyValue = 0x20;
			i=0;
			while(i<50 && !KEY2)
			{
				Delay10Ms(1);
				i++;
			}
		}
	}
	if(!KEY3)
	{
		Delay10Ms(1);
		if(!KEY3)
		{
			*keyValue = 0x30;
			i=0;
			while(i<50 && !KEY3)
			{
				Delay10Ms(1);
				i++;
			}
		}
	}	 	 
	if(!KEY4)
	{
		Delay10Ms(1);
		if(!KEY4)
		{
			*keyValue = 0x40;
			i=0;
			while(i<50 && !KEY4)
			{
				Delay10Ms(1);
				i++;
			}
		}
	}
	
}
