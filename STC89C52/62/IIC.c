#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
uchar temp,aa,bai,shi,ge;
sbit dula=P2^6;
sbit wela=P2^7;
sbit sda=P2^0;
sbit scl=P2^1;
uchar a;
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
void display(uchar bai,uchar shi,uchar ge);
void delay2(uint z);
void init();
void delay();
void start();
void stop();
void respons();
uchar read_byte();
void write_byte(uchar date);
void delay1(uchar x);
void write_add(uchar address,uchar date);
uchar read_add(uchar address);
void main()
{	
	init();//初始化子程序
	while(1)
	{
		if(aa==10)
		  {
			aa=0;
			temp++;
			if(temp==1000)
			   {
					temp=0;
			   }
			bai=temp/100;
			shi=temp%100/10;
			ge=temp%10;
			write_add(23,bai);
			delay1(10);
			write_add(24,shi);
			delay1(10);
			write_add(25,ge);
			delay1(10);
		  }
		  bai=read_add(23);
		  delay1(10);
		  shi=read_add(24);
		  delay1(10);
		  ge=read_add(25);
		  delay1(10);
		  display(bai,shi,ge);
	}
}

void delay2(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

void display(uchar bai,uchar shi,uchar ge)
{
        dula=1;
		P0=table[bai];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfe;
		wela=0;
		delay2(1);

		dula=1;
		P0=table[shi];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfd;
		wela=0;
		delay2(1);

		dula=1;
		P0=table[ge];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfb;
		wela=0;
		delay2(1);
}

void init()
{	
	wela=0;
	dula=0;
	//temp=0;
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;	

	sda=1;
	delay();
	scl=1;
	delay();

	bai=read_add(23);
	delay1(10);
	shi=read_add(24);
	delay1(10);
	ge=read_add(25);
	delay1(10);
	temp=bai*100+shi*10+ge;
}

void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	aa++;	
}
void delay()
{ ;; }
void start()  //开始信号
{	
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}

void stop()   //停止
{
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}

void respons()  //应答
{
	uchar i;
	scl=1;
	delay();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delay();
}

void write_byte(uchar date)
{
	uchar i,temp;
	temp=date;


	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
	    delay();
		sda=CY;
		delay();
		scl=1;
		delay();
	//	scl=0;
     //   delay();
	}
	scl=0;
	delay();
	sda=1;
	delay();
}

uchar read_byte()
{
	uchar i,k;
	scl=0;
	delay();
	sda=1;
	delay();
	for(i=0;i<8;i++)
	{
		scl=1;
		delay();	
		k=(k<<1)|sda;
		scl=0;
		delay();	
	}
	return k;
}

void delay1(uchar x)
{
	uchar a,b;
	for(a=x;a>0;a--)
	 for(b=100;b>0;b--);
}

void write_add(uchar address,uchar date)
{
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	write_byte(date);
	respons();
	stop();
}

uchar read_add(uchar address)
{
	uchar date;
	start();
	write_byte(0xa0);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(0xa1);
	respons();
	date=read_byte();
	stop();
	return date;
}
