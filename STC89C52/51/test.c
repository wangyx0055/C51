#include <reg52.h>
/*
*本文件是单片机数据采集和发送的入口部分，是整个硬件的主模块，调用
*DHT.H文件采集温度，调用SERIAL.H实现串口通信，调用LCD1602.H实现LCD的显示，
*在文件内部实现了GPRS通信
*/

#include <stdio.h>
#include <math.h>
#include "LCD1602.h"
#include "Serial.h"
//#include "DHT.h"

#define uchar unsigned char
#define uint unsigned int

//sbit pulse=P3^2;
//定义LCD显示的字符常量
uchar code str1[]="Current Temperat"; //第一行显示内容
uchar code str2[]="Current Humidity"; //第二行显示内容
uchar code str3[]="Current Dust";      //
//定义了SIM300模块GPRS通信的AT指令
uchar code s1[]="AT+CMGF=0\r\n";                          //设定Text模式
uchar code s2[]="AT+CMGS=27\r\n";                         //注意一定要加上 （ ;\r\n ） ；和换行、回车。
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //短信内容
uchar s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";   
//定义了与粉尘采集有关的变量
unsigned long xdata  duration;
unsigned long xdata starttime;
unsigned long code sampletime_ms = 5000;
unsigned long xdata lowpulseoccupancy = 0;
float idata ratio = 0;
float idata concentration = 0;
uchar xdata value[40]="hello!";
char xdata sensor[10];
uchar count,len,i;
//定义与GPRS通信有关的函数
//uchar THvalue[6];
void delay(int n);
void sendsms();
void DialVoiceCall();
void ConnectGPRS(float dust,float t,float h);
//定义与DHT11相关的变量
sbit P2_2 = P2^2 ;
uchar DHTFLAG,DHTtemp;
uchar DHT_data_H,DHT_data_L,DHTRH_data_H,DHTRH_data_L,DHTcheckdata;
uchar DHT_data_H_temp,DHT_data_L_temp,DHTRH_data_H_temp,DHTRH_data_L_temp,DHTcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[20];
float temperature,humidity;
//定义与DHT11相关的函数
void DHTDelay(uint z);
void DHTDelay_10us(void);
void DHTCOM(void);
void DHTCOM(void);
float ToDem(uchar x);
float binToDem(uchar x);
void RH(void);
//函数入口部分
void main()
{
	//uchar i;
	//EX1=1;
	//IT1=1;
	//初始化LCD和串口
	LCDInit();
	SerialInit();
	len=strlen(value);
	for(i=0;i<len;i++)
  		LCDWriteData(value[i]);
	//sendsms();
	//DialVoiceCall();
	//
	while(1)
	{
	//读取传感器数据，并发送到GPRS模块
		RH();
		SerialPutString(buff);
		 // SerialPutByte('\n');
		/*
		LCDWriteCom(0x01);//清屏
		LCDWriteCom(0x80);
		len=strlen(buff);
		for(i=0;i<len;i++)
	  		LCDWriteData(buff[i]);
			*/
	//	ConnectGPRS(0.3,0.4,0.8);
	}
}
/***********************************************
*                       延迟1ms函数
************************************************/
void delay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
/***********************************************
*                       发送短信
************************************************/
void sendsms()
{
SerialPutString(s1);
delay(2000);
SerialPutString(s2);
delay(3000);
SerialPutString(s3);
delay(2000);
SerialPutString(&s4);
delay(2000);
}
/***********************************************
*                       打电话
************************************************/
void DialVoiceCall()
{
  SerialPutString("AT+COLP=1\r\n");//dial the number
  delay(2000);
  SerialPutString("ATD15200831527;\r\n");//dial the number
  delay(2000);
}
/***********************************************
*                       GPRS采集数据
************************************************/
void ConnectGPRS(float dust,float t,float h)
{
 // char temp[40];
  /*
  SerialPutString("AT+CGCLASS=\"B\"\r\n");
  delay(2000);
  SerialPutString("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");
  delay(2000);
  SerialPutString("AT+CGATT=1\r\n");
  delay(2000);
  SerialPutString("AT+CIPCSGP=1,\"CMNET\"\r\n");
  delay(2000);
  SerialPutString("AT+CIPCSGP=0\r\n");
  delay(2000);
  SerialPutString("AT+CDNSCFG=\"211.91.216.129\"\r\n");
  delay(2000);
  */
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  delay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  delay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  delay(2000);
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f",dust);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",t);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",h);//
  SerialPutString(sensor);
  SerialPutByte(s4);
  delay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  delay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  delay(2000);
}
/******************************
*定义DHT延迟1s函数
********************************/
void DHTDelay(uint z)
{
uchar x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}
/******************************
*定义DHT延迟10us函数
********************************/
void DHTDelay_10us(void)
   {
     uchar i;
     i--;
     i--;
     i--;
     i--;
     i--;
     i--;
    }
/******************************
*定义DHT命令函数
********************************/
void DHTCOM(void)
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {

     DHTFLAG=2; 
     while((!P2_2)&&DHTFLAG++);
    DHTDelay_10us();
     DHTDelay_10us();
     DHTDelay_10us();
   DHTtemp=0;
     if(P2_2)DHTtemp=1;
     DHTFLAG=2;
    while((P2_2)&&DHTFLAG++);   
     if(DHTFLAG==1)break;    
     DHTcomdata<<=1;
     DHTcomdata|=DHTtemp; 
     }    
}
/******************************
*定义整型二进制转十进制函数
********************************/
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
	}
	return y;
}

/******************************
*定义浮点型二进制转十进制函数
********************************/
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
/******************************
*定义温度读取函数
********************************/
void RH(void)
{
      P2_2=0;
    DHTDelay(34);
    P2_2=1;
    DHTDelay_10us();
    DHTDelay_10us();
    DHTDelay_10us();
    DHTDelay_10us();
    P2_2=1;   
     if(!P2_2) 
     {
      DHTFLAG=2; 
      while((!P2_2)&&DHTFLAG++);
      DHTFLAG=2;
      while((P2_2)&&DHTFLAG++); 
      DHTCOM();
      DHTRH_data_H_temp=DHTcomdata;
	  //P1=DHTRH_data_H_temp;//
      DHTCOM();
      DHTRH_data_L_temp=DHTcomdata;
      DHTCOM();
      DHT_data_H_temp=DHTcomdata;
	  //
	  P1=DHT_data_H_temp;
      DHTCOM();
      DHT_data_L_temp=DHTcomdata;
	  //P1=DHT_data_L_temp;
      DHTCOM();
      DHTcheckdata_temp=DHTcomdata;
      P2_2=1; 
      DHTtemp=(DHT_data_H_temp+DHT_data_L_temp+DHTRH_data_H_temp+DHTRH_data_L_temp);
       if(DHTtemp==DHTcheckdata_temp)
       {
          DHTRH_data_H=DHTRH_data_H_temp;
          DHTRH_data_L=DHTRH_data_L_temp;
          DHT_data_H=DHT_data_H_temp;
          DHT_data_L=DHT_data_L_temp;
          DHTcheckdata=DHTcheckdata_temp;
		  humidity=ToDem(DHTRH_data_H_temp)+binToDem(DHTRH_data_L_temp);
		  temperature=ToDem(DHT_data_H_temp)+binToDem(DHT_data_L_temp);
		 // temperature=(int)DHTRH_data_H_temp;
		 // humidity=(int)DHT_data_H_temp;
		  sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);//
       }    
    }
}



