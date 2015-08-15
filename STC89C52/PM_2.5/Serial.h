#include<string.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int

uchar flag=0,a,i=0,len;//,num,temp,m,count;
uchar inbuff[32];
void SerialInit()
{
	TMOD=0x21;//设置定时器1为工作方式2,定时器0为工作1方式
	TH1=0xfd;
	TL1=0xfd;//
	TR1=1;
	TH0=0x00;
	TL0=0x00;//
	//
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	//
	memset(inbuff,0x00,32);
}

void SerialPutByte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}

void SerialPutEnd()
{
	SBUF=0x1a;
	while(!TI);
	TI=0;
}

void SerialPutString(uchar *puts)
{
	int j;
	int n;
	ES=0;
	n=strlen(puts);
	//short showdata=0;
	for(j=0;j<n;j++)
		SerialPutByte(puts[j]);
//	SerialPutByte('\n');
//	SerialPutByte('\r');
	memset(puts,0x00,32);
	ES=1;
}

void serialInt() interrupt 4
{
	if(RI)
	{
		RI=0;
		a=SBUF;
		if(a!='\n'||a!='\r')
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
	
}

uchar SerailRevCheck()
{
	return flag;
}

void SerialRevData(uchar buff[32])
{
	flag=0;
	len=strlen(inbuff);
	memcpy(buff,inbuff,strlen(inbuff));
	buff[strlen(inbuff)]='\0';
	memset(inbuff,0x00,32);

	//return buff;
}

uchar SerialGetDataLen()
{
	return len;
}


