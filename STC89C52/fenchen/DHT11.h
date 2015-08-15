#ifndef __DHT11_H__
#define __HDT11_H__

#include <intrins.h> 
#include <stdio.h>
#include <math.h>

#define uchar unsigned char
#define uint unsigned int

sbit P2_2 = P2^2 ;
uchar DHTFLAG,DHTtemp;
uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge;
uchar DHT_data_H,DHT_data_L,DHTRH_data_H,DHTRH_data_L,DHTcheckdata;
uchar DHT_data_H_temp,DHT_data_L_temp,DHTRH_data_H_temp,DHTRH_data_L_temp,DHTcheckdata_temp;
uchar DHTcomdata;
uchar xdata buff[20];
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
//
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
      DHTRH_data_H_temp=DHTcomdata;
	  //P1=DHTRH_data_H_temp;//
      DHTCOM();
      DHTRH_data_L_temp=DHTcomdata;
      DHTCOM();
      DHT_data_H_temp=DHTcomdata;
	  //
	  //P1=DHT_data_H_temp;
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
		 // sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);
       }    
    }
}

#endif 

