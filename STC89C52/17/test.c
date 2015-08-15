#include <reg52.h>
#include <intrins.h>

unsigned char code table[]={0x3f  , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
					   0x7d , 0x07 , 0x7f  , 0x6f , 0x77 , 0x7c ,
					   0x39 , 0x5e , 0x79 , 0x71};

sbit wei=P2^7;
sbit duan=P2^6;
unsigned char sec,shi,ge,flag;
void delay1();
void display1();
void init();
void display(unsigned char sec,unsigned char shi,unsigned char ge);
int count,m,num0,num1,sum;

void main()
{
	
	init();
	m=count=0;
	duan=1;
	P0=0x3f;
	duan=0;
	wei=1;
	P0=0xfe;
	wei=0;
	
	sum=433;
	flag=sec=shi=ge=0;
	while(1)
	{
		if(num1==10&&m<7)
		{
			num1=0;
			if(sum>398)
			{
				sum--;
			}
			else
			{
				flag=1;
			}
			sec=sum/100;
			shi=(sum%100)/10;
			ge=sum%10;
			//注意数码管显示函数放的位置
		}
		//display(sec,shi,ge);
		if(num0==50&&m<7)
		{
			num0=0;
			if(flag!=1)
			{
				P1=_crol_(0xfe,count);
				count++;
				count=count%8;
			}
			else
			{
					m++;
			}		
			
		}
		if(m>6)
		{
			P1=0xff;
			display1();
		}
		else
		{
			if(m%2==1)
				P1=0x00;
			else
				P1=0xff;
		display(sec,shi,ge);
		}
	//	display(count,shi,ge);
	}
}

void timeInt0() interrupt 1
{
	TH0=(65535-10000)>>8;
	TL0=(65535-10000)%256;
	num0++;
}

void timeInt1() interrupt 3
{
	TH1=(65535-10000)>>8;
	TL1=(65535-10000)%256;
	num1++;
}

void init()
{
	TMOD=0x11;
	TH1=(65535-10000)>>8;
	TL1=(65535-10000)%256;
	TH0=(65535-10000)>>8;
	TL0=(65535-10000)%256;
	EA=1;
	ET1=1;
	ET0=1;
	TR1=1;
	TR0=1;
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
			P0=table[6];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfe;
			wei=0;
			delay1();

			duan=1;
			P0=table[5];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfd;
			wei=0;
			delay1();

			duan=1;
			P0=table[4];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfb;
			wei=0;
			delay1();

			duan=1;
			P0=table[sec];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xf7;
			wei=0;
			delay1();
			//	
			duan=1;
			P0=table[shi];
			duan=0;
			P0=0xff;//消抖部分
			wei=1;
			P0=0xef;
			wei=0;
			delay1();
			//			
			duan=1;
			P0=table[ge];
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xdf;
			wei=0;
			delay1();
			//
	}
   void display1()
   {
   			duan=1;
			P0=0x76;
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfe;
			wei=0;
			delay1();

			duan=1;
			P0=0x79;
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfd;
			wei=0;
			delay1();

			duan=1;
			P0=0x38;
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xfb;
			wei=0;
			delay1();

			duan=1;
			P0=0x38;
			duan=0;	
			P0=0xff;//消抖部分
			wei=1;
			P0=0xf7;
			wei=0;
			delay1();
			//	
			duan=1;
			P0=0x3f;
			duan=0;
			P0=0xff;//消抖部分
			wei=1;
			P0=0xef;
			wei=0;
			delay1();
   }
