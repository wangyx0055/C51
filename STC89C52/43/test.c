#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "LCD1602.h"
#define uchar unsigned char
#define uint unsigned int

sbit pulse=P3^3;
uint  counter=0;
uchar len,i,count;
uchar buff[10]="hello";
uint lowpulseoccupancy;
uint code sampletime_ms = 3000;
float ratio,concentration,duration;
void delay(int n);
void init();

void main()
{
	LCDInit();
	init();
	//
	while(1)
	{
			//
		if(counter>299)
		{
			TR1=0;
			//P1=TL0;
	    	counter=0;
			duration=(float)lowpulseoccupancy;
			ratio = duration/(sampletime_ms*10);  // Integer percentage 0=>100
	        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
			sprintf(buff,"%f",ratio);
			lowpulseoccupancy = 0;
			//
			TR1=1;
			//
			len=strlen(buff);
			LCDWriteCom(0x01);
			LCDWriteCom(0x80);
			for(i=0;i<len;i++)
				LCDWriteData(buff[i]);
		}					
		//delay(2000);
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
	TMOD=0x11;
	TL0=0;
	TH0=0;
	TR0=0;
	TH1=0xd8; 
 	TL1=0xef; 
	TR1=1;
	//
	EA=1; 
	EX1=1;
	IT1=1;//负跳变沿
 	ET0=1;
 	ET1=1;

	//
}
//
void int1() interrupt 2 using 0
{
	TR0=1;
	while(pulse==0);
	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+(long)TL0;
	TR0=0;
	TL0=0;
}
//
void timer1() interrupt 3
{//timer0中断服务程序，中断一次延时10ms 
 counter++;//节拍次数计数  
 TH1=0xd8; //定义单位节拍的延时大小  
 TL1=0xef; 
 /*
 if(counter==300)
 	P1=0x0f;
 else
 	P1=0xff;
	*/
} 

