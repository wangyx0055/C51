#include <reg52.h>
unsigned char code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
sbit wei=P2^7;
sbit duan=P2^6;
sbit key1=P3^5;
unsigned char num;
void display();
void delay1();

void main()
{
	wei=1;
	P0=0xfe;
	wei=0;

	duan=1;
	P0=0x3f;
	duan=0;

	num=0;
	while(1)
	{	
		//防止低电平有突刺
		if(!key1)
		{
			delay1();
			if(!key1)
			{
				num++;
				num=num%16;
			}
		//防止高电平有凹谷
			while(!key1);
			delay1();
			while(!key1);
		}
		display();
	}
}

void display()
{
	duan=1;
	P0=table[num];
	duan=0;
}

void delay1()
{
	int i,j;
	for(i=5;i>0;i--)
		for(j=110;j>0;j--);
}

