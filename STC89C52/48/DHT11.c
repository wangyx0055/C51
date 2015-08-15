#include <reg51.h> 
#include <intrins.h> 
#include <math.h>    
#include <stdio.h>   
#define uchar unsigned char
#define uint unsigned int
#define   Data_0_time    4
uchar disdat[6];
uchar code reg[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  outdata[5];  //定义发送的字节数	   
U8  indata[5];
U8  count, count_r=0;

U16 U16temp1,U16temp2;
sbit P1_1=P2^2;
//void Delay(void);
void Delay_10us(void);
void delay(void);
void display(void);
void display1(void);
void delay(void)		//延时程序
	{
	uchar i;
	for (i=0;i<201;i++)
		_nop_();
	}
void Delay(U16 j)
    {      U8 i;
	    for(;j>0;j--)
	  { 	
		for(i=0;i<27;i++);

	  }
    }
       void  Delay_10us(void)
      {
        U8 i;
        i--;
        i--;
        i--;
        i--;
        i--;
        i--;
       }
void display(void)           //六位数码管显示程序，disdat[]是要显示的内容（是0～9的数字），xsd是在那一位显示小数点
	{
	uchar i,disa,disb,disc;
	disb = 0x00;		//显示第n位
	for (i=0;i<6;i++)		//共显示6个数据
		{
		disa = disdat[i];	//显示数据
		if (disa < 10)
			{
			P2 = disb;      //驱动数码管
			disc = reg[disa];  //显示段码
			if (i==1||i == 4)
			disc = disc | 0x80;		//增加小数点显示
			P0 = disc;				//送显示
			delay ();				//延时
			P0 = 0x00;              //
			disb++;					//下一个要显示的位置
			}
		}
	P2 = 0xff;
	}
/*void display1(void)
{uchar i,disa,disb,disc;
 disb = 0x00;
 for(i=0;i<6;i++)
 {disa=disdat[i];
 if(disa<10)
 {P2=disb;
  disc=reg[disa];
  if(i==2)
  disc=0x40;
  if(i==4)
  disc=disc | 0x80;
  P0=disc;
  delay ();				//延时
  P0 = 0x00;              //
  disb++;					//下一个要显示的位置
			}
		}
P2 = 0xff;
	}*/

void init_uart()
{SCON=0X52;
 TMOD=0X20;
 TCON=0X69;
 TH1 =0XFD;
}
void  COM(void)
      {
     
	    U8 i;
		U8comdata=0;
       for(i=0;i<8;i++)	   
	    {
		
	   	    U8FLAG=2;
	   /*	//----------------------
	         P1_1=0 ;  //T
	         P1_1=1 ;  //T
	    //---------------------- 
	*/
	   	while((!P1_1)&&U8FLAG++);
			Delay_10us();
			Delay_10us();
			Delay_10us();
	  		U8temp=0;
	     if(P1_1)
		 {U8temp=1;
		    U8FLAG=2;
		 while((P1_1)&&U8FLAG++);
		 }	
	   /* //----------------------
	          P2_1=0 ;  //T
	          P2_1=1 ;  //T
	   //----------------------*/
	   	//超时则跳出for循环		  
	   	 //if(U8FLAG==1)break;
	   	//判断数据位是0还是1	 
	   	   
		// 如果高电平高过预定0高电平值则数据位为 1 
	   	 else
		   U8temp=0;
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   //return(U8comdata);
	}
	
	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

	void RH(void)
	{
	  //主机拉低18ms 
       P1_1=0;
	   Delay(180);
	   P1_1=1;
	 //总线由上拉电阻拉高 主机延时40us
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	   Delay_10us();
	 //主机设为输入 判断从机响应信号 
	   P1_1=1;
	 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	   if(!P1_1)		 //T !	  
	   {
	   U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	   while((!P1_1)&&U8FLAG++);
	   U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	   while((P1_1)&&U8FLAG++);
	 //数据接收状态		 
	   COM();
	   U8RH_data_H_temp=U8comdata;
	   COM();
	   U8RH_data_L_temp=U8comdata;
	   COM();
	   U8T_data_H_temp=U8comdata;
	   COM();
	   U8T_data_L_temp=U8comdata;
	   COM();
	   U8checkdata_temp=U8comdata;
	   P1_1=1;
	   P1=U8comdata;
	 //数据校验 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }//fi
	   }//fi

	}
void main()
{
    unsigned char c;
	unsigned int a,i;
    init_uart();
	Delay(1);         //延时100US（12M晶振)
	while(1)
	{  

	   //------------------------
	   //调用温湿度读取子程序 
	   RH();
	   c=U8RH_data_H;
	   disdat[0]=c/10;
       disdat[1]=c%10;
	   disdat[2]=U8RH_data_L;
	   a=U8T_data_H;
       disdat[3]=a/10;
       disdat[4]=a%10;
       disdat[5]=U8T_data_L;
	   i=255;
	   while(i--)
       display(); //显示温湿度
	/*   //串口显示程序 
	   //--------------------------

	   str[0]=U8RH_data_H;
	   str[1]=U8RH_data_L;
	   str[2]=U8T_data_H;
	   str[3]=U8T_data_L;
	   str[4]=U8checkdata;
	   SendData(str) ;  //发送到串口  
	   //读取模块数据周期不易小于 2S */
	   //Delay(10);
	}//elihw
	
}// main

