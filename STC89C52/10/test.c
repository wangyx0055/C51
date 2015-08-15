#include <reg52.h>

void delay1();
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit beep=P2^3;
int count,num;
unsigned char table[]={0x3f  , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
					   0x7d , 0x07 , 0x7f  , 0x6f , 0x77 , 0x7c ,
					   0x39 , 0x5e , 0x79 , 0x71};

void main()
{
	EA=1;
	ET0=1;
	TMOD=0x01;
	TH0=(65535-50000)>>8;
	TL0=(65535-50000)%256;
	TR0=1;

	count=0;
	duan=1;
	P0=0xfa;//段选低电平有效
	duan=0;
	
	wei=1;
	P0=0x3f;//段选低电平有效
	wei=0;

	while(1)
	{
		if(num==40)
		{
			num=0;
			wei=1;
			P0=table[count++];
			wei=0;
			//
			if(count==16)
				beep=0;
			else
				beep=1;
			//
			count=count%16;		
			delay1();
		}
	}
}

void delay1()
{
	int i,j;
	for(i=250;i>0;i--)
		for(j=230;j>0;j--);
}

void show1()interrupt 1
{
	TH0=(65535-50000)>>8;
	TL0=(65535-50000)%256;
	num++;
}
