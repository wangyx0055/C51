#include <reg52.h>
#include <math.h>
#include <stdio.h>
#include "Serial.h"
#define uchar unsigned char
#define uint  unsigned int

sbit signal=P2^0;
uchar tempH,tempL,HumiH,HumiL,check;
uchar xdata buff[40];
float temperature,humidity;
void delay(int n);
void readByte(uchar x);
void delay_10us();
void binToDem(uchar x,float y);

void main()
{
	SerialInit();
	//初始化信号
	signal=1;
	delay(5);
	signal=0;
	delay(30);
	//开始信号
	signal=1;
	delay_10us();
	delay_10us();
	delay_10us();
	//读取DHT11响应信号
	if(!signal)
	{
		while(!signal);
		while(signal);
		readByte(HumiH);
		readByte(HumiL);
		readByte(tempH);
		readByte(tempL);
		readByte(check);
		signal=0;
		delay(1);
		signal=1;
		P1=HumiH;
		binToDem(HumiL,humidity);
		humidity=humidity+(int)HumiH;
		binToDem(tempL,temperature);
		temperature=temperature+(int)tempH;
		sprintf(buff,"%f,%f",humidity,temperature);
		SerialPutString(buff);
	}
	delay(3000);
//	while(1);
}
/***********************************************
*                       延迟1ms函数
************************************************/
void delay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/***********************************************
*                       延迟10us函数
************************************************/
void delay_10us()
{
	uchar i=10;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}

void readByte(uchar x)
{
	uchar i;
	bit temp;
	x=0;
	for(i=0;i<8;i++)
	{
		while(!signal);
		delay_10us();
		delay_10us();
		delay_10us();
		temp=0;
		if(signal==1)
			temp=1;
		while(signal);
		x=(x<<1);
		x=x|temp;		
	}
}
//
void binToDem(uchar x,float y)
{
	uchar i,m;
//	bit temp;
	m=0x80;
	y=0;
	for(i=0;i<8;i++)
	{
		if((x&(m>>i))>0)
			y=y+1/pow(2,i+1);
	}
}
