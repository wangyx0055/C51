#include <reg52.h>
#define uchar unsigned char
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

sbit duan=P2^6;
sbit wei=P2^7;
sbit key1=P3^4;

void delay1();
void init();
void display(uchar bai,uchar shi,uchar ge);
uchar aa,bai,shi,ge,value,flag;
int temp;

void main()
{
	init();
	
	while(1)
	{	
	  if(aa==2)
		{
		aa=0;
		P1=0xff;
		value=P3;
		value=value&0xf0;
		if(value!=0xf0)
		{
			delay1();
			value=P3;
			value=value&0xf0;
			if(value!=0xf0)
			{	
				//value=P3;
				P1=0xfe;
				switch(value)
				{
					case 0xe0:	break;
					case 0xd0:	TR0=0;flag=0;break;
					case 0xb0:	TR0=1;flag=1;break;
					case 0x70:	temp=0;break;
				}
			}
			while(value!=0xf0)
			{
				value=P3;
				value=value&0xf0;
			}
		}
		else
		{
			if(temp==1000)
			{
				temp=0;
			}		
			bai=temp/100;
			shi=(temp%100)/10;		
			ge=temp%10;
			if(flag)
				temp++;	
		}
		if(!flag)
		  aa=2;
	}
		display(bai,shi,ge);			  
	}
}

void init()
{
	wei=0;
	duan=0;
	temp=0;
	flag=1;

	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;	
	TR0=1;
}

void delay1()
{
	int i,j;
	for(i=1;i>0;i--)
		for(j=110;j>0;j--);
}

void display(uchar sec,uchar shi,uchar ge)
{
			duan=1;
			P0=table[sec];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfe;
			wei=0;
			delay1();
			//	
			duan=1;
			P0=table[shi];
			duan=0;
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfd;
			wei=0;
			delay1();
			//			
			duan=1;
			P0=table[ge];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfb;
			wei=0;
			delay1();
			//
	}

void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	aa++;
}

