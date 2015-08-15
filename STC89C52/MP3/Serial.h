#include<reg52.h>
#include<string.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int

uchar flag=0,a,i=0,len;//,num,temp,m,count;
uchar inbuff[64];
void SerialInit()
{
	TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;//
	TR1=1;
	//
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
	//
	memset(inbuff,0x00,64);
}

void SerialPutByte(uchar c)
{
	SBUF=c;
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
	SerialPutByte('\n');
//	SerialPutByte('\r');
	memset(puts,0x00,64);
	ES=1;
}

void serialInt() interrupt 4
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
	
}

uchar SerailRevCheck()
{
	return flag;
}

void SerialRevData(uchar buff[64])
{
	flag=0;
	len=strlen(inbuff);
	memcpy(buff,inbuff,strlen(inbuff));
	buff[strlen(inbuff)]='\0';
	memset(inbuff,0x00,64);

	//return buff;
}

uchar SerialGetDataLen()
{
	return len;
}

/*----------------------------------------------------
 函数名：ftoa()
 功能  ：将一个浮点数转为一个精度为jd的字符串，存于s中
------------------------------------------------------*/
char * ftoa(double dat,char *s,unsigned char jd)
{
 int len,temp,flag=dat<0?dat=-dat,1:0,i;
 char t[30];
 temp=(int)dat;
 for(len=0;temp>0;temp/=10,len++)
 t[len]=temp%10+48;
 for(i=0;i<=len;i++)
 s[len-i-1]=t[i];
 s[len++]='.';
 for(i=0,temp=(int)((dat-(int)dat)*pow(10,jd));temp>0;temp/=10,i++)
 t[i]=temp%10+48;
 for(i=0;i<jd;i++)
 s[len++]=t[jd-i-1];
 //s[len++]=0x0d;
 //s[len++]=0x0a;
 s[len]=0;
 return s;
}

/*--------------------------------------------
 函数名：szzh16()
 功能  ：将datax按16进制转成字符串，存于p中
---------------------------------------------*/

int szzh16(unsigned char datax, char *p)
{
 unsigned char temp;
 temp=datax;
 p[0]=((temp>>4)>=10)?(temp>>4)+55:(temp>>4)+48;
 p[1]=((temp&0x0f)>=10)?(temp&0x0f)+55:(temp&0x0f)+48;
 p[2]=0;
 return 1;
}

void PutLong(unsigned long dat)
{
unsigned long temp;
 while(dat)
 {
  temp=(dat%10+0x30);
  SerialPutString(&temp);
  dat/=10;
 }
}

void PutHex(unsigned char x)
{
 unsigned char temp[10];
 szzh16(x,temp);
 SerialPutString(temp);
}
//
void Printf(char *s,unsigned long d)
{
// unsigned char temp[30];
 SerialPutString(s);
 PutLong(d);
}
//
void send(unsigned char mydata)	
{
	SerialPutString(&mydata);
}
