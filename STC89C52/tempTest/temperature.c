#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define uchar unsigned char
#define uint unsigned int
sbit DS=P2^2;           //定义DS18B20数据引脚
sbit beep=P2^3;
uint temp;             // variable of temperature
float alarm=50.0;
uchar flag1,inbuff[10],buff[10],counter,start;            // 定义串口的收发缓存
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};
//定义延时函数
void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}
//功能:串口初始化,波特率9600，方式1
void Init_Com(void)
{
     TMOD = 0x20;
     PCON = 0x00;
     SCON = 0x50;
     TH1 = 0xFd;
     TL1 = 0xFd;
     TR1 = 1;
	 ES=1;
}
//DS18B20重置和初始化
void dsreset(void)       //  
{
  uint i;
  DS=0;
  i=103;
  while(i>0)i--;
  DS=1;
  i=4;
  while(i>0)i--;
}
//DS18B20读一比特位
bit tmpreadbit(void)       
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}
//DS18B20读一个字节
uchar tmpread(void)   
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //读出的数据最低位在最前面，这样刚好一个字节在DAT里
  }
  return(dat);
}
//写一个字节到DS18B20
void tmpwritebyte(uchar dat)  
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //写1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //写0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }

  }
}
//DS18B20温度读取准备
void tmpchange(void)  
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // skip ROM命令
  tmpwritebyte(0x44);  // 温度转换命令
}
//获取DS18B20温度值
uint tmp()               
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}

void readrom()          //read the serial
{
  uchar sn1,sn2;
  dsreset();
  delay(1);
  tmpwritebyte(0x33);
  sn1=tmpread();
  sn2=tmpread();
}


void delay10ms()            //delay
  {
    uchar a,b;
    for(a=10;a>0;a--)
      for(b=60;b>0;b--);
   }
//串口发送一个字符
void putbyte(uchar c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
//串口发送一个字符串
void putstring(uchar *puts,uchar n)
{	
	int j;
	ES=0;
	for(j=0;j<n;j++)
		putbyte(puts[j]);
    ES=1;
	memset(puts,0x00,10);
}
//设置发送数据格式
void sendData(uint temp)
{
	 float ser;
	 //float ser1[]={23.0,25.1,65.6,13.7,42.3};
   //int i;
   ser=(float)temp/10;
   sprintf(buff,"%f\n",ser);
   putstring(buff,strlen(buff));
  // SBUF=ser;
/*	
  for(i=0;i<5;i++){
   sprintf(buff,"%f\n",ser1[i]);
   SBUF=',';
   	while(!TI);
	TI=0;
   delay(1000);
   putstring(buff,strlen(buff));
   }
   */
   //
   SBUF=',';
   	while(!TI);
	TI=0;
}
//数码管显示温度值
void display(uint temp)			//显示程序
{
   uchar A1,A2,A2t,A3;
   float ser;
   ser=(float)temp/10;
   if(ser>alarm)
   	beep=0;
   else
    beep=1;
   //
   A1=temp/100;
   A2t=temp%100;
   A2=A2t/10;
   A3=A2t%10;
   dula=0;
   P0=table[A1];		//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7e;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];		//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0x7d;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];		//显示个位
   dula=1;
   dula=0;

   P0=0x7b;
   wela=1;
   wela=0;
   delay(1);
}

void init() 
{//定时器初始化 
 TMOD=0x01; 
 TH0=0xd8; 
 TL0=0xef;
 TR0=1;
 EA=1; 
 ET0=1;
 //
 start=0;
}  
//定时器0中断函数
void timer0() interrupt 1 using 3
{//timer0中断服务程序，中断一次延时10ms 
 counter++;//节拍次数计数  
 TH0=0xd8; //定义单位节拍的延时大小  
 TL0=0xef; 
} 
//串口中断函数
void serial() interrupt 4
{
	int i=0;
	uchar a;
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
			alarm=atof(inbuff);
			memset(inbuff,0x00,10);
		//	flag=1;
		}
	}
	
}

void main()
{
	 uchar a;
	 init();//初始化定时器
	 Init_Com();//初始化串口
	  do
	  {
	  //设定温度读取样周期
	   if(counter>200)
	   {
		counter=0;
	   	start=1;
	   }
	   //读取温度值，并发送到上位机
	   if(start)
	   {
	    start=0;
	    tmpchange();
		sendData(tmp());
		}
	    //数码管显示温度值
	  for(a=10;a>0;a--)
	  {   
	    display(tmp());
	  }
	  }
	  while(1);
}