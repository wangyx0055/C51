#include <reg52.h>
sbit buzzer=P2^3;
void delay(int x);
void main()
{
	buzzer=0;
	delay(1);
	buzzer=1;
	delay(1);
}

void delay(int x)
{
	int n,m;
	for(n=0;n<110;n++)
		for(m=0;m<x;m++);
}
