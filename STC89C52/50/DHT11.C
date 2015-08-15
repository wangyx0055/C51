#include <reg52.h>
#include <intrins.h> 
#include <stdio.h>
#include <math.h>
#include "Serial.h"

typedef unsigned char U8;
typedef unsigned int   U16;
U8 code table[]={0xc0,0xf9,0xa4,0xb0,
       0x99,0x92,0x82,0xf8,
       0x80,0x90,0x88,0x83,
       0xc6,0xa1};
sbit P2_0 = P2^2 ;
U8 U8FLAG,U8temp;
U8 shidu_shi,shidu_ge,wendu_shi,wendu_ge;
U8 U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8 U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8 U8comdata;
U8 xdata buff[20];
float temperature,humidity;
void Delay(U16 z)
{
U8 x,y;
for(x=z;x>0;x--)
   for(y=110;y>0;y--);
}

void Delay_10us(void)
   {
     U8 i;
     i--;
     i--;
     i--;
     i--;
     i--;
     i--;
    }

void COM(void)
{     
    U8 i;         
    for(i=0;i<8;i++)    
    {

     U8FLAG=2; 
     while((!P2_0)&&U8FLAG++);
    Delay_10us();
     Delay_10us();
     Delay_10us();
   U8temp=0;
     if(P2_0)U8temp=1;
     U8FLAG=2;
    while((P2_0)&&U8FLAG++);   
     if(U8FLAG==1)break;    
     U8comdata<<=1;
     U8comdata|=U8temp; 
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
      P2_0=0;
    Delay(34);
    P2_0=1;
    Delay_10us();
    Delay_10us();
    Delay_10us();
    Delay_10us();
    P2_0=1;   
     if(!P2_0) 
     {
      U8FLAG=2; 
      while((!P2_0)&&U8FLAG++);
      U8FLAG=2;
      while((P2_0)&&U8FLAG++); 
      COM();
      U8RH_data_H_temp=U8comdata;
	  //P1=U8RH_data_H_temp;//
      COM();
      U8RH_data_L_temp=U8comdata;
      COM();
      U8T_data_H_temp=U8comdata;
	  //
	  //P1=U8T_data_H_temp;
      COM();
      U8T_data_L_temp=U8comdata;
	  P1=U8T_data_L_temp;
      COM();
      U8checkdata_temp=U8comdata;
      P2_0=1; 
      U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
       if(U8temp==U8checkdata_temp)
       {
          U8RH_data_H=U8RH_data_H_temp;
          U8RH_data_L=U8RH_data_L_temp;
          U8T_data_H=U8T_data_H_temp;
          U8T_data_L=U8T_data_L_temp;
          U8checkdata=U8checkdata_temp;
		  humidity=ToDem(U8RH_data_H_temp)+binToDem(U8RH_data_L_temp);
		  temperature=ToDem(U8T_data_H_temp)+binToDem(U8T_data_L_temp);
		 // temperature=(int)U8RH_data_H_temp;
		 // humidity=(int)U8T_data_H_temp;
		  sprintf(buff,"%2.2f,%2.2f;",temperature,humidity);
       }    
    }
    //display(shidu_shi,shidu_ge,wendu_shi,wendu_ge);
}


void main()
{
	SerialInit();
	while(1)
	{ 
	    RH();
		SerialPutString(buff);
	}

}