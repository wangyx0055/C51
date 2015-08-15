/*********************************
*本文件是粉尘数据采集接口，定义了一些与粉尘采集有关的函数，通过
*调用本文件，能够对粉尘传感器进行一些基本操作
*********************************/
#ifndef __DUST_H__
#define __DUST_H__

#include <stdio.h>
#include <string.h>
#include <math.h>

#define uchar unsigned char
#define uint unsigned int
//定义了相关变量
sfr  T2MOD=0xC9;
sbit pulse=P3^2;
//sbit buzzer=P2^3;

uint  counter=0;
float lowpulseoccupancy;
uint code sampletime_ms = 3000;
float ratio,concentration;
/*********************************
*定义了延时1s函数
*********************************/
void Dustdelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/*********************************
*粉尘传感器数据读取初始化
*********************************/
void DustInit()
{
	lowpulseoccupancy = 0;
	//初始化定时器0
	TL0=0;
	TH0=0;
//	TR0=1;
	EX0=1;
	IT0=1;//负跳变沿
	//定时器2数据寄存器初始化
	RCAP2H=(65536-1000)/256;
	RCAP2L=(65536-1000)%256;
	T2CON=0;
	T2MOD=0;
	//开定时器中断和全局中断
	ET2=1;
	TR2=1;
	EA=1; 	
}
/*********************************
*定义外部中断函数
*********************************/
void int1() interrupt 0 using 3
{
	TR0=1;
	while(pulse==0);
	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
	TR0=0;
	TL0=0;
}
/*********************************
*定义定时器2中断函数
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}
/*********************************
*定义整型二进制转十进制函数
*********************************/
float DustToDem(uchar x)
{
	int i=0;
	float y=0;
	unsigned char m=0x01;
	if((x&m)==m)
		y=1;
	for(i=1;i<8;i++)
	{
		m=m<<1;
		if((x&m)==m)
			y=y+pow(2.0,i);
	//	printf("%f,%d\n",y,m);
	}
	return y;
}

#endif 
