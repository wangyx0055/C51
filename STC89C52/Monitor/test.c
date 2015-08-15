#include <reg52.h>
#include <stdio.h>
#include <math.h>
#include "LCD1602.h"
#include "Serial.h"
#include "DHT11.h"

#define uchar unsigned char
#define uint unsigned int

//sbit pulse=P3^2;

uchar code str1[]="Current Temperat"; //第一行显示内容
uchar code str2[]="Current Humidity"; //第二行显示内容
uchar code str3[]="Current Dust";      //
//
uchar code s1[]="AT+CMGF=0\r\n";                          //设定Text模式
uchar code s2[]="AT+CMGS=27\r\n";                         //注意一定要加上 （ ;\r\n ） ；和换行、回车。
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //短信内容
uchar s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";   
//
unsigned long idata  duration;
unsigned long idata starttime;
unsigned long idata sampletime_ms = 5000;
unsigned long idata lowpulseoccupancy = 0;
float idata ratio = 0;
float idata concentration = 0;
uchar idata value[40]="hello!";
char idata sensor[10];
uchar count,len,i;
//
//uchar THvalue[6];
void delay(int n);
void sendsms();
void DialVoiceCall();
void ConnectGPRS(float dust,float t,float h);

void main()
{
	//uchar i;
	//EX1=1;
	//IT1=1;
	LCDInit();
	SerialInit();
	len=strlen(value);
	for(i=0;i<len;i++)
  		LCDWriteData(value[i]);
	//用于发送短信
	//sendsms();
	//用于拨打电话
	//DialVoiceCall();
	//
	while(1)
	{
		
		RH();
		ConnectGPRS(0.3,0.4,0.8);
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

