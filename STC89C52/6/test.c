#include <reg52.h>
#include <intrins.h>

void delay1();
int m,n;
unsigned char temp1,temp2;

void main()
{
	temp1=0xfe;
	temp2=0x7f;
	//间隔300ms先奇数亮再偶数亮，循环三次
	for(m=0;m<3;m++)
	{
		P1=0xAA;
		delay1();
		P1=0x55;
		delay1();
	}
	//一个灯上下循环三次
	for(m=0;m<3;m++)
	{
		for(n=0;n<8;n++)
		{
			P1=temp1;
			temp1= _crol_(temp1,1);
			delay1();
		}
		for(n=0;n<8;n++)
		{
			P1=temp2;
			temp2= _cror_(temp2,1);
			delay1();
		}
	}
	//两个分别从两边往中间流动三次,再从中间往两边流动三次
	for(m=0;m<3;m++)
	{
		for(n=0;n<4;n++)
		{
			P1=temp1&temp2;
			temp1= _crol_(temp1,1);
			temp2= _cror_(temp2,1);
			delay1();
		}
		for(n=0;n<4;n++)
		{
			P1=temp1&temp2;
			temp1= _crol_(temp1,1);
			temp2= _cror_(temp2,1);
			delay1();
		}
	}
	//8个全部闪烁3次
	for(m=0;m<3;m++)
	{
		P1=0x00;
		delay1();
		P1=0xff;
		delay1();
	}
	//关闭发光管
	P1=0xff;
	delay1();
}

void delay1()
{
	int i,j;
	for(i=250;i>0;i--)
		for(j=230;j>0;j--);
}

