#include <reg52.h>
//sbit d0=P1^0;
void delay01()
{
	int i,j;
	for(i=0;i<500;i++)
		for(j=0;j<230;j++);
}
void main()
{
	while(1)
	{
		P1=0xFE;
		delay01();
		P1=0xFD;
		delay01();
		P1=0xFB;
		delay01();
		P1=0xF7;
		delay01();
		P1=0xEF;
		delay01();
		P1=0xDF;
		delay01();
		P1=0xBF;
		delay01();
		P1=0x7F;
		delay01();
	}
}
