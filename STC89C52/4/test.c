#include <reg52.h>
#include <intrins.h>
void delay1();
//sbit d1=P1^0;
sbit beep=P2^3;
unsigned char temp;
int count;

void main()
{
	temp=0xfe;
	count=0;
	while(1)
	{
		P1=temp;
		temp= _crol_(temp,1);
		if(count==0)
			delay1();
		beep=0;
		count++;
		count=count%7;
	}
}

void delay1()
{
	int i,j;
	for(i=50;i>0;i--)
		for(j=230;j>0;j--);
}
