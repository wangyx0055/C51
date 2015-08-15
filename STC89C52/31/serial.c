#include<reg52.h>
#include<string.h>
#define uchar unsigned char
#define uint unsigned int

uchar flag=0,a,i=0;
uchar inbuff[32];

void putbyte(uchar c);
void putstring(uchar *puts,uchar n);
void main()
{

	TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	memset(inbuff,0x00,32);
	while(1)
		{
		/*	if(RI==1)
			{
			RI=0;
			P1=SBUF;
			}*/
			//实现数据发送不使用中断，而数据接收使用中断
			if(flag==1)
				{
					ES=0;
					flag=0;
					SBUF='1';//28;	
					while(!TI);	
					TI=0;
					SBUF='2';//28;	
					while(!TI);	
					TI=0;
					SBUF='3';//28;	
					while(!TI);	
					TI=0;
					putstring(inbuff,strlen(inbuff));
					ES=1;
				}
		}
}

void ser() interrupt 4
{
	if(RI)
	{
		RI=0;
		a=SBUF;
		if(a!='\n')
		{
			inbuff[i]=a;
			i++;
		}
		else
		{
			i=0;
			flag=1;
		}
	}
	
//	P1=SBUF;
	
//	flag=1;
}

void putbyte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}

void putstring(uchar *puts,uchar n)
{	
	int j;
	for(j=0;j<n;j++)
		putbyte(puts[j]);
	memset(puts,0x00,32);
}
