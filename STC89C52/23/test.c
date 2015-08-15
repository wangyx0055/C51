#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit wr=P3^6;
sbit rd=P3^7;
void delay(uint z)
{
	uint t1,y;
	for(t1=z;t1>0;t1--)
		for(y=110;y>0;y--);
}
void init();
void start();
void main()
{
	init();
	while(1)
	{
		start();
		delay(20);
		rd=0;
       delay(10);
		rd=1;
		delay(10);
	}	
}

void init()
{
	P0=0;
}
void start()
{
	wr=1;
	wr=0;
	wr=1;	
}

