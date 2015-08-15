#include <stdio.h>
#include <math.h>
#include "Serial.h"

uchar code s1[]="AT+CMGF=0\r\n";                          //设定Text模式
uchar code s2[]="AT+CMGS=27\r\n";                         //注意一定要加上 （ ;\r\n ） ；和换行、回车。
uchar code s3[]="0011000D91685102801325F70008A00C62116B63572853D177ED4FE1";    //短信内容
uchar code s4=0x1a;                                       //Ctrl+Z 
uchar code s5[]="\r\n";   uchar xdata sensor[10];
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
 // char temp[40];
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
  SerialPutString("AT+CLPORT=\"UDP\",\"3033\"\r\n");//set a port
  GPRSdelay(2000);
  SerialPutString("AT+CIPSTART=\"UDP\",\"125.221.182.95\",\"8080\"\r\n");//set destination IP
  GPRSdelay(2000);
  SerialPutString("AT+CIPSEND\r\n");//dial the number
  GPRSdelay(2000);
  //mySerial.print("PM2.5 value:");
  sprintf(sensor,"%.2f",dust);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",t);//
  SerialPutString(sensor);
  sprintf(sensor,",%.2f",h);//
  SerialPutString(sensor);
  SerialPutByte(s4);
  GPRSdelay(2000);
  SerialPutString("AT+CIPCLOSE\r\n");//dial the number
  GPRSdelay(2000);
  SerialPutString("AT+CIPSHUT\r\n");
  GPRSdelay(2000);
}

