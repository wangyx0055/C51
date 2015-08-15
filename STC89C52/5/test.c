#include <reg52.h>
#include <intrins.h>

unsigned char temp,start;
int m,n;
void delay1();

void main()
{
	temp=0xff;
	for(m=1;m<9;m++)
	{
		start=temp<<m;
		for(n=0;n<9-m;n++)
		{
			P1=start;
			delay1();
			start= _crol_(start,1);
		}
	}
	
}

void delay1()
{
	int i,j;
	for(i=150;i>0;i--)
		for(j=230;j>0;j--);
}

