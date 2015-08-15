#include <reg52.h>
#include <intrins.h>

unsigned char temp;
int num;
void main()
{
	EA=1;
	ET1=1;
	TMOD=0x10;
	TH1=(65535-50000)>>8;
	TL1=(65535-50000)%256;
	TR1=1;
	
	temp=0xfe;
	while(1)
	{
		if(num==2)
		{
			num=0;
			P1=temp;
			temp= _crol_(temp,1);
		}
	}
}

void show1()interrupt 3
{
	TH1=(65535-50000)>>8;
	TL1=(65535-50000)%256;
	num++;
}
