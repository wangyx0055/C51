/*
*本文件是单片机数据采集和发送的入口部分，是整个硬件的主模块，调用DUST.H文件
*实现PPD42NS对粉尘的采集，调用SERIAL.H实现串口通信，调用LCD.H实现1602LCD的显示，调用
*GPRS.H文件实现了SIM300模块GPRS数据发送和通信
*/

#include <reg52.h>
#include <intrins.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Serial.h"
#include "LCD.h"
#include "GPRS.h"

#define uchar unsigned char
#define uint unsigned int

sbit P2_2 = P2^2 ;
//定义与温度采集有关的变量
uchar DHTFLAG,DHTtemp;
//uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge;
uchar idata DHT_data_H,DHT_data_L,DHRH_data_H,DHRH_data_L,ucharcheckdata;
uchar idata DHT_data_H_temp,DHT_data_L_temp,DHRH_data_H_temp,DHRH_data_L_temp,ucharcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[10];
//定义LCD显示的字符常量
uchar code info1[]="Temp:";
uchar code info2[]="Humid:";
uchar code info3[]="Dust:";
float idata temperature,humidity;
//定义粉尘数据采集的相关变量
sfr  T2MOD=0xC9;
sbit pulse=P3^2;
uint  counter=0;
float volatile lowpulseoccupancy;
uint code sampletime_ms = 6000;
float ratio,concentration;
/*********************************
*定义延迟1s函数
*********************************/
void Delay(uint z)
{
uchar x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}
/*********************************
*定义延迟10us函数
*********************************/
void Delay_10us(void)
   {
     uchar i;
     i--;
     i--;
     i--;
     i--;
     i--;
     i--;
    }
/*********************************
*定义温度采集的指令函数
*********************************/
void DHTCOM(void)
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {

     DHTFLAG=2; 
     while((!P2_2)&&DHTFLAG++);
    Delay_10us();
     Delay_10us();
     Delay_10us();
   DHTtemp=0;
     if(P2_2)DHTtemp=1;
     DHTFLAG=2;
    while((P2_2)&&DHTFLAG++);   
     if(DHTFLAG==1)break;    
     DHTcomdata<<=1;
     DHTcomdata|=DHTtemp; 
     }    
}
/*********************************
*定义整型二进制转十进制函数
*********************************/
float ToDem(uchar x)
{
	int i=0;
	float y=0;
	unsigned char m=0x01;
	if((x&m)==m)
		y=1;
	for(i=1;i<8;i++)
	{
		m=m<<1;
		if((x&m)==m)
			y=y+pow(2.0,i);
	//	printf("%f,%d\n",y,m);
	}
	return y;
}
/*********************************
*定义浮点型二进制转十进制函数
*********************************/
float binToDem(uchar x)
{
	float y=0;
	uchar i,m;
	m=0x80;	
	for(i=0;i<4;i++)
	{
		if((x&(m>>i))>0)
			y=y+1/pow(2.0,i+1);
	}
	return y;
}
/*********************************
*定义DS18B20温度读取函数
*********************************/
void RH(void)
{
	//根据温度传感器读数据的时序图写相应的协议
    P2_2=0;
    Delay(34);
    P2_2=1;
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    P2_2=1;   
     if(!P2_2) 
     {
      DHTFLAG=2; 
      while((!P2_2)&&DHTFLAG++);
      DHTFLAG=2;
      while((P2_2)&&DHTFLAG++); 
      DHTCOM();
      DHRH_data_H_temp=DHTcomdata;
	  //P1=DHRH_data_H_temp;//
      DHTCOM();
      DHRH_data_L_temp=DHTcomdata;
      DHTCOM();
      DHT_data_H_temp=DHTcomdata;
	  //
	  //P1=DHT_data_H_temp;
      DHTCOM();
      DHT_data_L_temp=DHTcomdata;
	 // P1=DHT_data_L_temp;
      DHTCOM();
      ucharcheckdata_temp=DHTcomdata;
      P2_2=1; 
      DHTtemp=(DHT_data_H_temp+DHT_data_L_temp+DHRH_data_H_temp+DHRH_data_L_temp);
	  //如果校验和正确，则赋给相应的变量
       if(DHTtemp==ucharcheckdata_temp)
       {
          DHRH_data_H=DHRH_data_H_temp;
          DHRH_data_L=DHRH_data_L_temp;
          DHT_data_H=DHT_data_H_temp;
          DHT_data_L=DHT_data_L_temp;
          ucharcheckdata=ucharcheckdata_temp;
		  humidity=ToDem(DHRH_data_H_temp)+binToDem(DHRH_data_L_temp);
		  temperature=ToDem(DHT_data_H_temp)+binToDem(DHT_data_L_temp);
		 // temperature=(int)DHRH_data_H_temp;
		 // humidity=(int)DHT_data_H_temp;
		  sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);
       }    
    }
//	return buff;
}
/*********************************
*粉尘传感器读取初始化
*********************************/
void DustInit()
{
	lowpulseoccupancy = 0;
	//定时器0数据寄存器初始化
	TL0=0;
	TH0=0;
//	TR0=1;
	EX0=1;
	IT0=1;//负跳变沿
	//定时器2数据寄存器初始值的载入
	RCAP2H=(65536-1000)/256;
	RCAP2L=(65536-1000)%256;
	T2CON=0;
	T2MOD=0;
	//开定时器中断和全局中断
	ET2=1;
	TR2=1;
	EA=1; 	
}
/*********************************
*定义外部中断函数
*********************************/
void int1() interrupt 0 using 3
{
	TR0=1;
	while(pulse==0);
//	P1=TL0;
	lowpulseoccupancy = lowpulseoccupancy+ToDem(TL0);
	TR0=0;
	TL0=0;
}
/*********************************
*定义定时器2的中断函数
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}
/***********************************************
*                       GPRS采集数据
注：读取扩展ram的数据不能通过传变量的方式，必须直接引用，否则会出现接收不到的现象
************************************************/
void SendGPRS()
{
	//初始化SIM300模块
	uchar idata sensor[10];
  //char temp=',';
  SerialPutString("AT+CGCLASS=\"B\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CGATT=1\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CIPCSGP=1,\"CMNET\"\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CIPCSGP=0\r\n");
  GPRSdelay(2000);
  SerialPutString("AT+CDNSCFG=\"211.91.216.129\"\r\n");
  GPRSdelay(2000);
	//按顺序发送UDP通信的AT指令
  memset(sensor,0x00,10);
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  GPRSdelay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  GPRSdelay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  GPRSdelay(2000);
  //发送数据
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f",concentration);//
  SerialPutString(sensor);
  memset(sensor,0x00,10);
  sprintf(sensor,",%.2f",temperature);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  SerialPutString(sensor);
  memset(sensor,0x00,10);
  sprintf(sensor,",%.2f",humidity);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  SerialPutString(sensor);
  //发送数据结束，断开连接
  SerialPutByte(s4);
  SerialPutByte(s4);
  GPRSdelay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  GPRSdelay(2000);
}
/*********************************
*主函数入口
*********************************/
void main()
{
	//定义数据采集的中间变量
	uchar len,i;
	uchar xdata Tvalue[10],Hvalue[10],Dvalue[10];
	//初始化串口
	SerialInit();
	//初始化LCD
	LCDInit();
	//初始化粉尘传感器
	DustInit();
	sendsms();
	while(1)
	{ 
	    //判断采样间隔值是否达到
		if(counter>sampletime_ms)
		{
		//读取温度数据
			RH();
			TR2=0;
			EX0=0;
			//SerialPutStr(buff);
			memset(Tvalue,0x00,10);
			memset(Hvalue,0x00,10);
			memset(Dvalue,0x00,10);
			//将浮点型转为字符串
			sprintf(Tvalue,"%2.2f",temperature);
			sprintf(Hvalue,"%2.2f%%",humidity);
			//读取粉尘数据
	    	counter=0;
		//	P1=0XAF;
			ratio = lowpulseoccupancy*100/sampletime_ms;  // Integer percentage 0=>100
	        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
			sprintf(Dvalue,"%.2f",concentration);
			//将数据通过GPRS模块发送出去
			//ConnectGPRS(Tvalue,Hvalue,Dvalue);
			SendGPRS();
		    //SerialPutStr(buff);
			lowpulseoccupancy = 0;
		    //TR2=1;

			//LCD清屏，并更新显示
			LCDWriteCom(0x01);//清屏
			LCDWriteCom(0x80);
			len=strlen(info1);
			//写数据到LCD1602
			for(i=0;i<len;i++)
				LCDWriteData(info1[i]);
			len=strlen(Tvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Tvalue[i]);
			LCDWriteData((uchar)223);
			LCDWriteData('C');
			LCDWriteCom(0x80+0x43);
			len=strlen(info2);
			for(i=0;i<len;i++)
				LCDWriteData(info2[i]);
			len=strlen(Hvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Hvalue[i]);
		   //
		    //Delay(3000);
			GPRSdelay(2000);
			LCDWriteCom(0x01);//清屏
			LCDWriteCom(0x80);
			len=strlen(info3);
			//写数据到LCD1602
			for(i=0;i<len;i++)
				LCDWriteData(info3[i]);
			len=strlen(Dvalue);
			for(i=0;i<len;i++)
				LCDWriteData(Dvalue[i]);
		   //
		    TR2=1;
		   	EX0=1;
		}	
	}

}