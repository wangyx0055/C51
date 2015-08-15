#include <reg52.h>
#include <intrins.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Serial.h"
#include "LCD.h"

#define uchar unsigned char
#define uint unsigned int

sbit P2_2 = P2^2 ;

uchar DHTFLAG,DHTtemp;
//uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge;
uchar idata DHT_data_H,DHT_data_L,DHRH_data_H,DHRH_data_L,ucharcheckdata;
uchar xdata DHT_data_H_temp,DHT_data_L_temp,DHRH_data_H_temp,DHRH_data_L_temp,ucharcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[20];
uchar code info1[]="Temp:";
uchar code info2[]="Humid:";
float temperature,humidity;

void Delay(uint z)
{
uchar x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}

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
//
//
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

//
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
//-----------------------
void RH(void)
{
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


void main()
{
	uchar len,i;
	uchar xdata Tvalue[6],Hvalue[6];
	SerialInit();
	LCDInit();
	while(1)
	{ 
	    RH();
		SerialPutString(buff);
		sprintf(Tvalue,"%2.2f",temperature);
		sprintf(Hvalue,"%2.2f%%",humidity);
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
	   Delay(10000);
	}

}