#include <reg52.h>
#include <intrins.h>

unsigned char code table1[]={0xbf  , 0x86 , 0xdb , 0xcf , 0xe6 , 0xed ,
					   0xfd , 0x87 , 0xff  , 0xef , 0xf7 , 0xfc ,
					   0xb9 , 0xde , 0xf9 , 0xf1};

unsigned char code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

sbit wei=P2^7;
sbit duan=P2^6;
unsigned char sec,shi,ge;
void delay1();
void display(unsigned char sec,unsigned char shi,unsigned char ge);
int count,m,num;

void main()
{
	TMOD=0x10;
	TH1=(65535-1000)>>8;
	TL1=(65535-1000)%256;
	EA=1;
	ET1=1;
	TR1=1;

	m=count=0;
	duan=1;
	P0=0x3f;
	duan=0;
	wei=1;
	P0=0xfe;
	wei=0;

	shi=ge=0;
	while(1)
	{
		if(num==10)
		{
			num=0;
			if(m==100)
			{
				m=0;
				count++;
			}
			shi=m/10;
			ge=m%10;		
			m++;
			count=count%16;
			display(count,shi,ge);
		}
		//display(count,shi,ge);
	}
}

void timeInt() interrupt 3
{
	TH1=(65535-1000)>>8;
	TL1=(65535-1000)%256;
	num++;
}

void delay1()
{
	int i,j;
	for(i=1;i>0;i--)
		for(j=110;j>0;j--);
}

void display(unsigned char sec,unsigned char shi,unsigned char ge)
{
			duan=1;
			P0=table1[sec];
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
