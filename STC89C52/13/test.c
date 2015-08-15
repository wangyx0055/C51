#include <reg52.h>
#include <intrins.h>

sbit  wei=P2^6;
sbit  duan=P2^7;
unsigned char temp;
int count,num,m,i;
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
	m=0;
	duan=1;
	P0=temp=0xfe;//段选低电平有效
	duan=0;
	
	wei=1;
	P0=0x3f;//段选低电平有效
	wei=0;

	while(1)
	{
		if(num==10)
		{
			num=0;
			duan=1;
			P0= _crol_(temp,m);//段选低电平有效
			duan=0;

			wei=1;
			P0=table[count++];
			wei=0;			
			//
			m++;
			m=m%6;
			count=count%16;		
			
		}
	}
}

void show1()interrupt 1
{
	TH0=(65535-50000)>>8;
	TL0=(65535-50000)%256;
	num++;
}
