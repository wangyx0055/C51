#include<reg52.h>
#include<string.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int

uchar flag=0,a,i=0,len;//,num,temp,m,count;
uchar inbuff[32],buff[32];
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;
uchar count;

void delay(int n);
void writeCom(uchar com);
void writeData(uchar value);
void init();
void putbyte(uchar c);
void putstring(uchar *puts,uchar n);
//int getIndex(uchar val);
void main()
{
	TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;//
	TR1=1;
	//
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	//
	memset(inbuff,0x00,32);
	init();
	
	while(1)
		{
		
			//实现数据发送不使用中断，而数据接收使用中断
			if(flag==1)
			{
				flag=0;					
				ES=0;
				len=strlen(inbuff);
				memcpy(buff,inbuff,strlen(inbuff));
				buff[strlen(inbuff)]='\0';
				putstring(inbuff,strlen(inbuff));
				ES=1;
				//
				writeCom(0x01);//清屏
				writeCom(0x80);
				for(count=0;count<len;count++)
				{
					if(count==15)
						writeCom(0x80+0x40);
					writeData(buff[count]);	
					delay(20);
				}
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
/*
int getIndex(uchar val)
{
	if(val>='0'&&val<='9')
		return val-'0';
	if((val>='A'&&val<='F'))
		return val-'A'+10;
	if(val>='a'&&val<='f')
		return val-'a'+10;
		
}*/
void init()
{
	duan=0;
	wei=0;
	lcden=0;
	count=0;
	writeCom(0x38);
	writeCom(0x0c);
	writeCom(0x06);
	writeCom(0x01);//清屏
	writeCom(0x80);
}
void delay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
void writeCom(uchar com)
{
	lcdrs=0;
	P0=com;
	delay(1);
	lcden=1;
	delay(5);
	lcden=0;
}
void writeData(uchar value)
{
	lcdrs=1;
	P0=value;
	delay(1);
	lcden=1;
	delay(5);
	lcden=0;
}
