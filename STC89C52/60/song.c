#include<reg52.h> 
#include<string.h>  
#include "Serial.h"        
#define uchar unsigned char 
#define uint unsigned int 
#define SONGNUM 6

sfr  T2MOD=0xC9;
sbit sound=P2^3; 
uint counter=0,sound_pace,record=0,k=0,count,m=1; 
uchar flag=0,len; 
uchar sound_signalhigh,sound_signallow;//定时器的高8位与低8位 
//定义节拍计数器 
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;
uchar code *song[]={"Molihua","Huluwa","Throw","ReadBoy","TwoTiger","HappyNewYear"};
uchar code *info[]={"Curr:","   Next:"};
short code index[]={0,68,167,271,352,385};
//
void lcdelay(int n);
void writeCom(uchar com);
void writeData(uchar value);
void init();
void delay(uchar n);
void sound_delay(uchar sound_signalhigh,uchar sound_signallow);
uint getNext(uint x);
uint getForward(uint x);
//

#define NOTE_END 0x0000
#define NOTE_SONGEND 0x0001
//定义E调的高、中、低音
#define NOTE_E0 0xffff
#define NOTE_E1 0xfa8c
#define NOTE_E2 0xfadc
#define NOTE_E3 0xfa6c
#define NOTE_E4 0xfaec
#define NOTE_E5 0xfc5e
#define NOTE_E6 0xfcc4
#define NOTE_E7 0xfd1e

#define NOTE_EL1 0xf518
#define NOTE_EL2 0xf5b7
#define NOTE_EL3 0xf6d1
#define NOTE_EL4 0xf7db
#define NOTE_EL5 0xf8be
#define NOTE_EL6 0xf987
#define NOTE_EL7 0xfa3c

#define NOTE_EH1 0xfd47
#define NOTE_EH2 0xfd6e
#define NOTE_EH3 0xfdb6
#define NOTE_EH4 0xfdf6
#define NOTE_EH5 0xfe2f
#define NOTE_EH6 0xfe62
#define NOTE_EH7 0xfe8f
//定义D调的高、中、低音
#define NOTE_D0 0xffff
#define NOTE_D1 0xf9e1
#define NOTE_D2 0xfa8c
#define NOTE_D3 0xfadc
#define NOTE_D4 0xfb6c
#define NOTE_D5 0xfbec
#define NOTE_D6 0xfc5e
#define NOTE_D7 0xfcc4

#define NOTE_DL1 0xf3c2
#define NOTE_DL2 0xf518
#define NOTE_DL3 0xf5b7
#define NOTE_DL4 0xf6d1
#define NOTE_DL5 0xf7db
#define NOTE_DL6 0xf8be
#define NOTE_DL7 0xf987

#define NOTE_DH1 0xfcf2
#define NOTE_DH2 0xfd47
#define NOTE_DH3 0xfd6e
#define NOTE_DH4 0xfdb6
#define NOTE_DH5 0xfdf6
#define NOTE_DH6 0xfe2f
#define NOTE_DH7 0xefee

//定义C调的高、中、低音
#define NOTE_C0 0xffff
#define NOTE_C1 0xf922
#define NOTE_C2 0xf9e1
#define NOTE_C3 0xfa8c
#define NOTE_C4 0xfadc
#define NOTE_C5 0xfb6c
#define NOTE_C6 0xfbec
#define NOTE_C7 0xfc5e

#define NOTE_CL1 0xf243
#define NOTE_CL2 0xf3c2
#define NOTE_CL3 0xf518
#define NOTE_CL4 0xf5b7
#define NOTE_CL5 0xf6d1
#define NOTE_CL6 0xf7db
#define NOTE_CL7 0xf8be

#define NOTE_CH1 0xfc93
#define NOTE_CH2 0xfcf2
#define NOTE_CH3 0xfd47
#define NOTE_CH4 0xfd6e
#define NOTE_CH5 0xfdb6
#define NOTE_CH6 0xfdf6
#define NOTE_CH7 0xfe2f


uint code tune[]= 
{
 //茉莉花
  NOTE_E3,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E5,NOTE_E6,
  NOTE_E5,NOTE_E3,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_EH1,NOTE_E6,
  
  NOTE_E5,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_E5,NOTE_E3,NOTE_E5,NOTE_E6,
  NOTE_E6,NOTE_E5,
  
  NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E1,NOTE_E2,NOTE_E1,
  NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E3,NOTE_E2,NOTE_E3,
  
  NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E1,
  NOTE_EL6,NOTE_EL5,NOTE_EL6,NOTE_E1,
  
  NOTE_E2,NOTE_E3,NOTE_E1,NOTE_E2,NOTE_E1,NOTE_EL6,NOTE_EL5,NOTE_SONGEND,
  //葫芦娃
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,NOTE_D0,NOTE_D6,
  NOTE_DH1,NOTE_D6,NOTE_D5,NOTE_DH1,NOTE_D6,NOTE_D0,
  
  NOTE_D1,NOTE_D1,NOTE_D3,
  NOTE_D1,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_D6,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D6,
  NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D7,NOTE_D7,NOTE_D5,NOTE_D3,
  NOTE_D5,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D3,NOTE_D0,
  NOTE_DH1,NOTE_D0,NOTE_D6,NOTE_D6,NOTE_D5,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D0,NOTE_D5,NOTE_D1,NOTE_D2,NOTE_D0,
  NOTE_D3,NOTE_D3,NOTE_D1,NOTE_DL6,
  NOTE_D1,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_D3,NOTE_D5,NOTE_D6,NOTE_D6,
  NOTE_DH1,NOTE_D0,NOTE_D7,NOTE_D5,
  NOTE_D6,NOTE_SONGEND,
 //丢手绢
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_EH1,NOTE_EH2,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E0,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,
  NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E3,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_EH1,NOTE_EH2,NOTE_EH1,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_EH1,NOTE_E0,
  NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,
  NOTE_E5,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,
  NOTE_E5,NOTE_E0,NOTE_E5,NOTE_E0,
  NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E0,
  NOTE_E5,NOTE_E5,NOTE_E3,NOTE_E6,NOTE_E5,
  NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E2,
  NOTE_E3,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_E2,
  NOTE_E3,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,
  NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E5,NOTE_E6,NOTE_E5,
  NOTE_E2,NOTE_E3,NOTE_E1,NOTE_E1,NOTE_SONGEND,
 //读书郎
  NOTE_EL6,NOTE_E1,NOTE_EL6,NOTE_E5,NOTE_EL6,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_EL6,NOTE_E1,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_E1,NOTE_E3,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,
  NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_EL6,NOTE_E1,NOTE_EL6,NOTE_E5,NOTE_EL6,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_EL6,NOTE_E1,NOTE_E2,NOTE_E3,NOTE_E2,NOTE_E1,NOTE_EL6,
  NOTE_EL6,NOTE_E1,NOTE_E3,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E5,NOTE_E3,
  NOTE_E3,NOTE_E5,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,
  NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E6,NOTE_E5,NOTE_E3,NOTE_E2,NOTE_SONGEND,
 //两只老虎
  NOTE_C1,NOTE_C2,NOTE_C3,NOTE_C1,NOTE_C1,NOTE_C2,NOTE_C3,NOTE_C1,
  NOTE_C3,NOTE_C4,NOTE_C5,NOTE_C3,NOTE_C4,NOTE_C5,
  NOTE_C5,NOTE_C6,NOTE_C5,NOTE_C4,NOTE_C3,NOTE_C1,NOTE_C5,NOTE_C6,NOTE_C5,NOTE_C4,NOTE_C3,NOTE_C1,
  NOTE_C1,NOTE_C5,NOTE_C1,NOTE_C1,NOTE_C5,NOTE_C1,NOTE_SONGEND,
 //新年好
  NOTE_E1,NOTE_E1,NOTE_E1,NOTE_EL5,NOTE_E3,NOTE_E3,NOTE_E3,NOTE_E1,
  NOTE_E1,NOTE_E3,NOTE_E5,NOTE_E5,NOTE_E4,NOTE_E3,NOTE_E2,
  NOTE_E2,NOTE_E3,NOTE_E4,NOTE_E4,NOTE_E3,NOTE_E2,NOTE_E3,NOTE_E1,
  NOTE_E1,NOTE_E3,NOTE_E2,NOTE_EL5,NOTE_EL7,NOTE_E2,NOTE_E1,NOTE_END//NOTE_SONGEND
  };
  //
float code duration[]=
{
	//茉莉花
  1,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,0.5,2,1,1,1,0.5,0.5,1,1,2,
  1,0.5,0.5,1,0.5,0.5,1,0.5,0.5,2,0.5,0.5,0.5,0.5,1.5,0.5,
  1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,2,1,1,
  1.5,0.5,0.5,0.5,0.5,0.5,3,1,
  //葫芦娃
  1,1,0.5,0.5,1,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,1,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,1,
  
  1,1,1+1,
  0.5,1,1+0.5,1,
  1,1,0.5,0.5,1,
  0.5,1,1+0.5,1,
  0.5,0.5,0.5,0.5,1+1,
  0.5,1,1+0.5,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  0.5,0.5,0.5+0.25,0.25,0.5+0.25,0.25,0.5+0.25,0.25,
  0.5,1,0.5,1,1,
  1+1,0.5,0.5,1,
  1+1+1+1,
  0.5,1,0.5,1+1,
  0.5,1,0.5,1+1,
  1+1,0.5,0.5,1,
  1+1+1+1,1,
//丢手绢
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,0.5,2,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,1,1,
  0.25,0.25,0.25,0.25,0.5,0.5,0.5,0.5,1,
  0.5,0.5,0.5,0.5,2,
  1,1,1,1,
  0.5+0.5,0.25,0.5,0.5,1,1,
  0.5,1,0.5,1,1,
  0.5+0.5,0.25,0.5,0.5,1,1,
  1,1,0.5,0.5,0.5,0.5,
  2,0.5,0.5,0.5,0.5,
  0.5,0.5,1,0.5,0.5,0.5,0.5,
  1,1,2,2,1,
 //读书郎
  1,0.25,0.5,0.5,1,0.25,1,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,1,
  1,0.25,1,0.5,0.25,0.25,0.5,0.5,
  0.25,0.25,0.25,0.25,0.25,0.25,0.5,2,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.25,0.5,0.5,1,0.25,1,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,1,
  1,0.25,1,0.5,0.25,0.25,0.5,0.5,
  0.25,0.25,0.25,0.25,0.25,0.25,0.5,2,
  0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,1,
 //两只老虎
  1,1,1,1,1,1,1,1,
  1,1,2,1,1,2,
  1,0.25,1,0.25,1,1,1,0.25,1,0.25,1,1,
  1,1,2,1,1,2,1,
 //新年好
   0.5,0.5,1,1,0.5,0.5,1,1,
   0.5,0.5,1,1,0.5,0.5,2,
   0.5,0.5,1,1,0.5,0.5,1,1,
   0.5,0.5,1,1,0.5,0.5,2,1
  };
  
void main() 
{  
 init(); 
 SerialInit();
 while(1) 
   { 
      k=0; 
	  init();
	  sound=1;
   while(tune[k]!=NOTE_END)//因为歌曲的结尾处人为的加入了 结束标示符  0  
   { 
   //检测串口是否发送控制信息过来，如果有，执行命令，否则继续
   	if(SerailRevCheck())
	{
		switch(getIndex())
		{
			case 200:sound=1;continue;//暂停播放歌曲
			case 201:sound=1;SerailSetCheck();break;//恢复播放歌曲
			case 202:sound=0;continue;//蜂鸣器报警
			case 203:sound=1;SerailSetCheck();break;//蜂鸣器停止报警
			case 204:P1=0x00;SerailSetCheck();break;//控制灯亮
			case 205:P1=0xff;SerailSetCheck();break;//控制灯灭
			case 206:sound=1;k=getNext(k);SerailSetCheck();break;//下一首歌
			case 207:sound=1;k=getForward(k);SerailSetCheck();break;//前一首歌
			default:
				sound=1;
				k=index[getIndex()];
				SerailSetCheck();
				//更新LCD显示内容
				m=getIndex();
				writeCom(0x01);//清屏
				writeCom(0x80);
				//
				 len=strlen(info[0]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(info[0][count]);	
					delay(20);
				 }
				 len=strlen(song[m]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(song[m][count]);	
					delay(20);
				 }
				 //
				 m++;
				 m=m%SONGNUM;
				 writeCom(0x80+0x40);
				 len=strlen(info[1]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(info[1][count]);	
					delay(20);
				 }
				 len=strlen(song[m]);
				 for(count=0;count<len;count++)
				 {
				 	writeData(song[m][count]);	
					delay(20);
				 }

		}
	}
	/*从表中取得 音符音高，也就是这个音符是低音，中音还是高
音,有了这两个参数我们就可以确定音符对应的定时器初值在表中的位置了。 */
    if(tune[k]==NOTE_SONGEND)
	{
		k++;
		sound=1;
		writeCom(0x01);//清屏
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
	
	}
    sound_signalhigh=0xff&(tune[k]>>8); 
    sound_signallow=0xff&tune[k]; 
    //从表中取得 节拍大小  
    sound_pace=40*duration[k];//600为一拍的时长 ,在此修改一拍的时长。 
	k++;
    TR2=1; 
   //当节拍数未达到时候，继续循环，产生该音调的声音  
   while(counter!=sound_pace) 
      {  
       sound=~sound; 
       sound_delay(sound_signalhigh,sound_signallow); 
	  // pwm_delay(temp);
      } 
     counter=0;//节拍计数器置0，进入下一个音调  
     TR2=0; 
    } 
   delay(100); //歌曲演唱完毕后,延时一段时间     }  
} 
}
//
void init() 
{//定时器初始化 
	 TMOD=0x21; 
	// TH0=0xd8; 
	// TL0=0xef;  
	 ET0=1;
	// ET1=1;
	//定时器2
	RCAP2H=(65536-10000)/256;
	RCAP2L=(65536-10000)%256;
	T2CON=0;
	T2MOD=0;

	ET2=1;
	EA=1;
	//TR2=1;
	 duan=0;
	wei=0;
	lcden=0;
	count=0;
	writeCom(0x38);
	writeCom(0x0c);
	writeCom(0x06);
	writeCom(0x01);//清屏
	writeCom(0x80);
//
 len=strlen(info[0]);
 for(count=0;count<len;count++)
 {
 	writeData(info[0][count]);	
	delay(20);
 }
 len=strlen(song[0]);
 for(count=0;count<len;count++)
 {
 	writeData(song[0][count]);	
	delay(20);
 }
 //
 writeCom(0x80+0x40);
 len=strlen(info[1]);
 for(count=0;count<len;count++)
 {
 	writeData(info[1][count]);	
	delay(20);
 }
 len=strlen(song[1]);
 for(count=0;count<len;count++)
 {
 	writeData(song[1][count]);	
	delay(20);
 }
 P1=0xff;
}  

void timer0() interrupt 1 using 3
{//timer0中断服务程序，中断一次延时10ms 
/*
 counter++;//节拍次数计数  
 TH0=0xd8; //定义单位节拍的延时大小  
 TL0=0xef; 
 */
	flag=1;
} 
/*
void timer1() interrupt 3
{//timer1中断服务程序
 flag=1;
} 
*/
/*********************************
*
*********************************/
void timer2() interrupt 5
{
	TF2=0;
	counter++;	
}

void delay(uchar n) 
{ //延时n毫秒
 uchar i; 
 while(n--) 
  for(i=0;i<125;i++);  
} 

void sound_delay(uchar sound_signalhigh,uchar sound_signallow)//产生一定频率的音调 
{  
 TH0=sound_signalhigh; 
 TL0=sound_signallow; 
 TR0=1; 
 while(!flag);
 	flag=0;
	TR0=0; 
} 

uint getNext(uint x)
{
	uchar i;
	for(i=0;i<SONGNUM;i++)
	{
		if(x<index[i])
		{
		//更新LCD显示内容
		m=i;
		writeCom(0x01);//清屏
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }

			return index[i];
			}
	}
		//
		m=0;
		writeCom(0x01);//清屏
		writeCom(0x80);
		//
		 len=strlen(info[0]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[0][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }
		 //
		 m++;
		 m=m%SONGNUM;
		 writeCom(0x80+0x40);
		 len=strlen(info[1]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(info[1][count]);	
			delay(20);
		 }
		 len=strlen(song[m]);
		 for(count=0;count<len;count++)
		 {
		 	writeData(song[m][count]);	
			delay(20);
		 }


	return 0;
}

uint getForward(uint x)
{
	uchar i;
	for(i=SONGNUM-1;i>0;i--)
	{
		if(x>index[i])
		{
			//更新LCD显示内容
			m=i-1;
			writeCom(0x01);//清屏
			writeCom(0x80);
			//
			 len=strlen(info[0]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[0][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }
			 //
			 m++;
			 m=m%SONGNUM;
			 writeCom(0x80+0x40);
			 len=strlen(info[1]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[1][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }

			return index[i-1];
		}
	}
			//
			m=SONGNUM-1;
			writeCom(0x01);//清屏
			writeCom(0x80);
			//
			 len=strlen(info[0]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[0][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }
			 //
			 m++;
			 m=m%SONGNUM;
			 writeCom(0x80+0x40);
			 len=strlen(info[1]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(info[1][count]);	
				delay(20);
			 }
			 len=strlen(song[m]);
			 for(count=0;count<len;count++)
			 {
			 	writeData(song[m][count]);	
				delay(20);
			 }

	return index[SONGNUM-1];
}
//
void lcdelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}
void writeCom(uchar com)
{
	lcdrs=0;
	P0=com;
	lcdelay(1);
	lcden=1;
	lcdelay(5);
	lcden=0;
}
void writeData(uchar value)
{
	lcdrs=1;
	P0=value;
	lcdelay(1);
	lcden=1;
	lcdelay(5);
	lcden=0;
}


 