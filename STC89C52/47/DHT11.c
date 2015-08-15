#include <reg52.h>
#include <intrins.h> 
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
void display(U8 shidu_shi,U8 shidu_ge,U8 wendu_shi,U8 wendu_ge)
{
P2=0xff;
P2=0xbf;
P0=table[shidu_shi];
Delay(5);
P2=0xff;
P2=0xdf;
P0=table[shidu_ge];
Delay(5);
P2=0xff;
P2=0xfb;
P0=table[wendu_shi];
Delay(5);
P2=0xff;
P2=0xfd;
P0=table[wendu_ge];
Delay(5);
}
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
	  P1=U8RH_data_H_temp;//
      COM();
      U8RH_data_L_temp=U8comdata;
      COM();
      U8T_data_H_temp=U8comdata;
      COM();
      U8T_data_L_temp=U8comdata;
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
       }
       shidu_shi=U8T_data_H/10; 
         shidu_ge=U8T_data_H%10; 
         wendu_shi=U8RH_data_H/10; 
         wendu_ge=U8RH_data_H%10;        
    }
    //display(shidu_shi,shidu_ge,wendu_shi,wendu_ge);
}
void main()
{
while(1)
{ 
    RH();
}

}