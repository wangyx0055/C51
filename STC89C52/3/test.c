#include <reg52.h>
#include <intrins.h>
void delay1();
//sbit d1=P1^0;
sbit beep=P2^3;
unsigned char temp;

void main()
{
	temp=0xfe;
	while(1)
	{
		beep=0;
		P1=temp;
		temp= _crol_(temp,1);
		delay1();
		beep=1;
		P1=0xff;
		delay1();
	}
}

void delay1()
{
	int i,j;
	for(i=250;i>0;i--)
		for(j=230;j>0;j--);
}
