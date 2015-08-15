/*
*本文件是AT24C02存储的IIC通信协议接口，定义了一些与IIC通信有关的函数，通过
*调用本文件，能够对AT24C02存储模块进行一些基本操作
*/
#ifndef __AT24C_H__
#define __AT24C_H__

#define uint unsigned int
#define uchar unsigned char
//定义IIC的时钟和数据线变量
sbit sda=P2^0;
sbit scl=P2^1;

/*********************************
*定义脉冲延时函数
*********************************/
void storeDelay()
{ ;; }
/*********************************
*初始化IIC通信协议
*********************************/
void initStore()
{
	sda=1;
	storeDelay();
	scl=1;
	storeDelay();
}
/*********************************
*定义协议开始信号函数
*********************************/
void storeStart()  //开始信号
{	
	sda=1;
	storeDelay();
	scl=1;
	storeDelay();
	sda=0;
	storeDelay();
}
/*********************************
*定义协议停止函数
*********************************/
void storeStop()   //停止
{
	sda=0;
	storeDelay();
	scl=1;
	storeDelay();
	sda=1;
	storeDelay();
}
/*********************************
*定义协议应答函数
*********************************/
void storeRespons()  //应答
{
	uchar i;
	scl=1;
	storeDelay();
	//通过延时的方式来进行应答
	while((sda==1)&&(i<250))i++;
	scl=0;
	storeDelay();
}
/*********************************
*定义字节写入函数
*********************************/
void storeWriteByte(uchar date)
{
	uchar i,temp;
	temp=date;

	//实现位的写入
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
	    storeDelay();
		sda=CY;
		storeDelay();
		scl=1;
		storeDelay();
	//	scl=0;
     //   storeDelay();
	}
	scl=0;
	storeDelay();
	sda=1;
	storeDelay();
}
/*********************************
*定义字节读取函数
*********************************/
uchar storeReadByte()
{
	uchar i,k;
	scl=0;
	storeDelay();
	sda=1;
	storeDelay();
	//实现位的读取
	for(i=0;i<8;i++)
	{
		scl=1;
		storeDelay();	
		k=(k<<1)|sda;
		scl=0;
		storeDelay();	
	}
	return k;
}
/*********************************
*定义写地址函数
*********************************/
void storeWriteAdd(uchar address,uchar date)
{
	storeStart();
	storeWriteByte(0xa0);
	storeRespons();
	storeWriteByte(address);
	storeRespons();
	storeWriteByte(date);
	storeRespons();
	storeStop();
}
/*********************************
*定义读地址函数
*********************************/
uchar storeReadAdd(uchar address)
{
	uchar date;
	storeStart();
	storeWriteByte(0xa0);
	storeRespons();
	storeWriteByte(address);
	storeRespons();
	storeStart();
	storeWriteByte(0xa1);
	storeRespons();
	date=storeReadByte();
	storeStop();
	return date;
}

#endif
