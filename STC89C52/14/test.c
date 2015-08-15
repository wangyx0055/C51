#include <reg52.h>
#include <intrins.h>

unsigned char code table[]={0x06 , 0x5b , 0x4f , 0x66 , 0x6d , 0x7d };
int count,num,m;
sbit wei=P2^7;
sbit duan=P2^6;
unsigned char temp;
void delay1();

void main()
{
	TMOD=0x01;
	TH0=(65535-1000)>>8;
	TL0=(65535-1000)%256;
	EA=1;
	ET0=1;
	TR0=1;

	count=0;
	wei=1;
	P0=temp=0xfe;
	wei=0;
	duan=1;
	P0=0xff;
	duan=0;

	while(1)
	{
		if(num==1)
		{
			num=0;
			wei=1;
			P0=_crol_(temp,count);
			wei=0;			
		
			P0=0xff;//Ïû¶¶²¿·Ö

			duan=1;
			P0=table[5-count];
			duan=0;

			count++;
			count=count%6;
			delay1();
		}
	}
}

void timeInt() interrupt 1
{
	TH0=(65535-1000)>>8;
	TL0=(65535-1000)%256;
	num++;
}

void delay1()
{
	int i,j;
	for(i=1;i>0;i--)
		for(j=110;j>0;j--);
}

