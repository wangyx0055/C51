#include<reg52.h>
sbit csda=P3^2;
sbit wr=P3^6;
sbit duan=P2^6;
sbit wei=P2^7;
unsigned char i,temp;
void delay();
void main()
{
	csda=0;
	wr=0;
	i=0;
	duan=0;//目的在于使数码管不亮
	wei=0;
	//P0=0;
	while(1)
	{
		if(i==8)
		{
			i=0;
			temp=0xff;
		}
		P0=temp>>i;	
		delay();
		i++;
	}
}

void delay()
{
	int i,j;
	for(i=20;i>0;i--)
		for(j=110;j>0;j--);
}
