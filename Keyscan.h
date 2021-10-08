#ifndef _KEYSCAN_H_
#define _KEYSCAN_H_

#include "reg52.h" 

sbit KEY1 = P2^5;
sbit KEY2 = P2^6;
sbit KEY3 = P2^7;
sbit KEY4 = P3^3;

void Key_Scan(unsigned char *keyValue);	//°´¼üÉ¨Ãè³ÌÐò

#endif
