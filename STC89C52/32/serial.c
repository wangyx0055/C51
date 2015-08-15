#include<reg52.h>
#include<string.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

uchar flag=0,a,i=0,num,temp,m,count;
uchar inbuff[32],buff[5];
sbit  wei=P2^6;
sbit  duan=P2^7;
unsigned char table[]={0x3f  , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
					   0x7d , 0x07 , 0x7f  , 0x6f , 0x77 , 0x7c ,
					   0x39 , 0x5e , 0x79 , 0x71};

void putbyte(uchar c);
void putstring(uchar *puts,uchar n);
int getIndex(uchar val);
void main()
{
	ET0=1;
	TMOD=0x21;//设置定时器1为工作方式2
	TH0=(65535-50000)>>8;
	TL0=(65535-50000)%256;	//
	TH1=0xfd;
	TL1=0xfd;//
	TR0=1;//
	TR1=1;
	//
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	//
	duan=1;
	P0=temp=0xfe;//段选低电平有效
	duan=0;
	
	wei=1;
	P0=0x3f;//段选低电平有效
	wei=0;
	
	m=num=0;
	memset(inbuff,0x00,32);
	while(1)
		{
			//实现数据发送不使用中断，而数据接收使用中断
			if(flag==1)
				{
				flag=0;
				
					
				ES=0;
			//	flag=0;
				memcpy(buff,&inbuff[2],4);
				putstring(inbuff,strlen(inbuff));
				ES=1;
				
				}
				if(num==5)
				{
					num=0;
					duan=1;
					P0= _crol_(temp,m);//段选低电平有效
					duan=0;
		
					wei=1;
					P0=table[getIndex(buff[m])];
					wei=0;			
					//
					m++;
					m=m%4;		
					
				}
		}
}

void serial() interrupt 4
{
	if(RI)
	{
		RI=0;
		a=SBUF;
		if(a!='\n')
		{
			inbuff[i]=a;
			i++;
		}
		else
		{
			i=0;
			flag=1;
		}
	}
	
//	P1=SBUF;
	
//	flag=1;
}

void Timershow()interrupt 1
{
	TH0=(65535-500)>>8;
	TL0=(65535-500)%256;
	num++;
}

void putbyte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}

void putstring(uchar *puts,uchar n)
{	
	int j;
	//short showdata=0;
	for(j=0;j<n;j++)
		putbyte(puts[j]);
	memset(puts,0x00,32);
}
int getIndex(uchar val)
{
	if(val>='0'&&val<='9')
		return val-'0';
	if((val>='A'&&val<='F'))
		return val-'A'+10;
	if(val>='a'&&val<='f')
		return val-'a'+10;
		
}
