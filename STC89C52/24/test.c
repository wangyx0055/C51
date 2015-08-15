#include <reg52.h>
#define uchar unsigned char
sbit duan=P2^6;
sbit wei=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;

void delay(int n);
void writeCom(uchar com);
void writeData(uchar value);
void init();
uchar  code table[]="I LOVE CHNINA!";
uchar count;
void main()
{
	init();
	//writeData('1');
	//writeData('A');
	for(count=0;count<14;count++)
	{
		writeData(table[count]);	
		delay(200);
	}
	while(1);

}
void init()
{
	duan=0;
	wei=0;
	lcden=0;
	count=0;
	writeCom(0x38);
	writeCom(0x0f);
	writeCom(0x06);
	writeCom(0x01);//ÇåÆÁ
	writeCom(0x80);
}
void delay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
void writeCom(uchar com)
{
	lcdrs=0;
	P0=com;
	delay(1);
	lcden=1;
	delay(5);
	lcden=0;
}
void writeData(uchar value)
{
	lcdrs=1;
	P0=value;
	delay(1);
	lcden=1;
	delay(5);
	lcden=0;
}
