#include <reg52.h>

void delay1();
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit d1=P1^1;
int count;
unsigned char table[]={0x3f  , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
					   0x7d , 0x07 , 0x7f  , 0x6f , 0x77 , 0x7c ,
					   0x39 , 0x5e , 0x79 , 0x71};

void main()
{
	EA=1;
	EX0=1;
	//TCON=0x01;

	count=0;
	duan=1;
	P0=0xfa;//段选低电平有效
	duan=0;
	
	while(1)
	{
		d1=1;
		wei=1;
		P0=table[count++];
		wei=0;
		//
		count=count%16;		
		delay1();
	}
}

void delay1()
{
	int i,j;
	for(i=250;i>0;i--)
		for(j=230;j>0;j--);
}

void showlight() interrupt 0
{
	d1=0;
}
