#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Serial.h"
#define uchar unsigned char
#define uint unsigned int

sfr  T2MOD=0xC9;
sbit pulse=P3^2;
//sbit buzzer=P2^3;

uint  counter=0;
uchar len,i,count;
uchar xdata buff[20];
float lowpulseoccupancy;
uint code sampletime_ms = 3000;
float ratio,concentration,duration;
void delay(int n);
void init();
float ToDem(uchar x);

void main()
{
	SerialInit();
	init();
	//
	while(1)
	{
	/*
			while(pulse==0);
			TL0=0;
			while(pulse==1);
			P1=TH0;
			lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
				//
				*/
			if(counter>sampletime_ms)
			{
			//	P1=TL0;
			//	buzzer=0;
				TR2=0;
		    	counter=0;
			//	P1=0XAF;
				ratio = lowpulseoccupancy*100/sampletime_ms;  // Integer percentage 0=>100
		        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
				sprintf(buff,"%.2f,%.2f,%.2f;",lowpulseoccupancy,ratio,concentration);
				SerialPutStr(buff);
				lowpulseoccupancy = 0;
				TR2=1;
				//
			}			
	}
}
//
void delay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
//
void init()
{
	lowpulseoccupancy = 0;
	//
	TL0=0;
	TH0=0;
//	TR0=1;

	//

	EX0=1;
	IT0=1;//负跳变沿
	//定时器2
	RCAP2H=(65536-1000)/256;
	RCAP2L=(65536-1000)%256;
	T2CON=0;
	T2MOD=0;

	ET2=1;
	TR2=1;
	EA=1; 
//	
}
//
void int1() interrupt 0 using 3
{
	TR0=1;
	while(pulse==0);
	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
	TR0=0;
	TL0=0;
}

//
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}

float ToDem(uchar x)
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

