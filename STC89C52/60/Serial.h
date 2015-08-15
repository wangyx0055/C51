#ifndef __SERIAL_H__
#define __SERIAL_H__

#include<string.h>
#include<intrins.h>
#include <stdlib.h>
#define uchar unsigned char
#define uint unsigned int

uchar Serialflag=1,a,i=0,len;
long position;//,num,temp,m,count;
uchar xdata inbuff[32];
/***********************************************
*                       串口初始化
************************************************/
void SerialInit()
{
	TMOD=0x21;//设置定时器1为工作方式2
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
	position=200;
	Serialflag=1;
	memset(inbuff,0x00,32);
}
/***********************************************
*                       串口发送字符
************************************************/
void SerialPutByte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
/***********************************************
*                       串口发送字符串
************************************************/
void SerialPutString(uchar *puts)
{
	int j;
	int n;
	ES=0;
	n=strlen(puts);
	//short showdata=0;
	for(j=0;j<n;j++)
		SerialPutByte(puts[j]);
//	SerialPutByte('\n');
//	SerialPutByte('\r');
//	memset(puts,0x00,32);
	ES=1;
}
/***********************************************
*                       串口发送以'\n'结尾的字符串
************************************************/
void SerialPutStr(uchar *puts)
{
	int j;
	int n;
	ES=0;
	n=strlen(puts);
	//short showdata=0;
	for(j=0;j<n;j++)
		SerialPutByte(puts[j]);
	SerialPutByte('\n');
//	SerialPutByte('\r');
//	memset(puts,0x00,32);
	ES=1;
}
/***********************************************
*                       串口中断函数
************************************************/
void serialInt() interrupt 4
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
			inbuff[i]='\0';
			i=0;
			Serialflag=1;
		}
	}
	//P1=0x00;
	position=atoi(inbuff);
}
/***********************************************
*                       
************************************************/
long getIndex()
{
	return position;
}
/***********************************************
*                       串口检测是否有数据
************************************************/
uchar SerailRevCheck()
{
	return Serialflag;
}
/***********************************************
*                       串口检测是否有数据
************************************************/
void SerailSetCheck()
{
	Serialflag=0;
}
/***********************************************
*                       串口接收数据
************************************************/
void SerialRevData(uchar buff[32])
{
	Serialflag=0;
	len=strlen(inbuff);
	memcpy(buff,inbuff,strlen(inbuff));
	buff[strlen(inbuff)]='\0';
	memset(inbuff,0x00,32);

	//return buff;
}
/***********************************************
*                       串口获取接收数据长度
************************************************/
uchar SerialGetDataLen()
{
	return len;
}

#endif 
