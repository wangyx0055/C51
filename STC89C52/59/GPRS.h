/*********************************
*本文件是GPRS通信接口，定义了一些与GPRS通信有关的函数，通过
*调用本文件，能够对GPRS通信进行一些基本操作
*********************************/
#ifndef __GPRS_H__
#define __GPRS_H__

#include <stdio.h>
#include <math.h>
//#include "Serial.h"
//定义GPRS通信的AT指令
uchar code s1[]="AT+CMGF=0\r\n";                          //设定Text模式
uchar code s2[]="AT+CMGS=27\r\n";                         //注意一定要加上 （ ;\r\n ） ；和换行、回车。
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //短信内容
uchar code s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";
uchar xdata sensor[30];
/***********************************************
*                       延迟1ms函数
************************************************/
void GPRSdelay(int n)
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
	GPRSdelay(2000);
	SerialPutString(s2);
	GPRSdelay(3000);
	SerialPutString(s3);
	GPRSdelay(2000);
	SerialPutString(&s4);
	GPRSdelay(2000);
}
/***********************************************
*                       打电话
************************************************/
void DialVoiceCall()
{
  SerialPutString("AT+COLP=1\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("ATD15200831527;\r\n");//dial the number
  GPRSdelay(2000);
}
/***********************************************
*                       GPRS采集数据
************************************************/
void ConnectGPRS(float dust,float t,float h)
{
  //初始化GPRS
  /*
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
  */
  //建立UDP通信
  memset(sensor,0x00,30);
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  GPRSdelay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  GPRSdelay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  GPRSdelay(2000);
  //发送数据
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f,%.2f,%.2f",dust,t,h);//
  SerialPutString(sensor);
  //memset(sensor,0x00,10);
  //sprintf(sensor,",%.2f",t);//
  //GPRSdelay(200);
  //SerialPutByte(',');
  //SerialPutString(t);
  //memset(sensor,0x00,10);
  //sprintf(sensor,",%.2f",h);//
  //GPRSdelay(200);
  //SerialPutByte(',');
 // SerialPutString(h);
 //结束数据发送，并断开连接
  SerialPutByte(s4);
  SerialPutByte(s4);
  GPRSdelay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  GPRSdelay(2000);
}

#endif 
