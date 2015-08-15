/*********************************
*本文件是LCD1602显示接口，定义了一些与LCD1602显示有关的函数，通过
*调用本文件，能够对LCD1602进行一些基本操作
*********************************/
#ifndef __LCD_H__
#define __LCD_H__

#define uchar unsigned char
#define uint unsigned int
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;
/***********************************************
*                       LCD延迟1ms函数
************************************************/
void LCDDelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/***********************************************
*                       LCD写命令函数
************************************************/
void LCDWriteCom(uchar com)
{
	lcdrs=0;
	P0=com;
	LCDDelay(1);
	lcden=1;
	LCDDelay(5);
	lcden=0;
}
/***********************************************
*                       LCD写数据函数
************************************************/
void LCDWriteData(uchar value)
{
	lcdrs=1;
	P0=value;
	LCDDelay(1);
	lcden=1;
	LCDDelay(5);
	lcden=0;
}
/***********************************************
*                       LCD初始化函数
************************************************/
void LCDInit()
{
	duan=0;
	wei=0;
	lcden=0;
	//count=0;
	LCDWriteCom(0x38);
	LCDWriteCom(0x0c);
	LCDWriteCom(0x06);
	LCDWriteCom(0x01);//清屏
	LCDWriteCom(0x80);
}
#endif 
