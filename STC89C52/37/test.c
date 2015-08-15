#include<reg52.h>         
#define uchar unsigned char 
#define uint unsigned int 

sbit sound=P2^3; 
uint counter=0; 
//
#define NOTE_END 0x0000
#define NOTE_SONGEND 0x0001
//
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
//以上部分是定义是把每个音符和频率值对应起来，其实不用打这么多，但是都打上了，后面可以随意编写D调的各种歌，我这里用NOTE_D+数字表示音符，NOTE_DH+数字表示上面有点的那种音符，NOTE_DL+数字表示下面有点的那种音符。这样后面写起来比较好识别。
uchar code tune[]= 
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
  
  NOTE_E2,NOTE_E3,NOTE_E1,NOTE_E2,NOTE_E1,NOTE_EL6,NOTE_EL5,NOTE_END
};
//
float code duration[]=
{
	//茉莉花
  1,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,
  1,0.5,0.5,2,1,1,1,0.5,0.5,1,1,2,
  1,0.5,0.5,1,0.5,0.5,1,0.5,0.5,2,0.5,0.5,0.5,0.5,1.5,0.5,
  1,0.5,0.5,2,1,0.5,0.5,0.5,0.5,0.5,0.5,2,1,1,
  1.5,0.5,0.5,0.5,0.5,0.5,3,1
};
//
void init() 
{//定时器初始化 
 TMOD=0x11; 
 TH0=0xd8; 
 TL0=0xef; 
 EA=1; 
 ET0=1; 
}  

void timer0(void) interrupt 1 using 3  
{//timer0中断服务程序，中断一次延时10ms 
 counter++;//节拍次数计数  
 TH0=0xd8; //定义单位节拍的延时大小  
 TL0=0xef; 
} 

void delay(uchar n) 
{ //延时n毫秒
 uchar i; 
 while(n--) 
  for(i=0;i<125;i++);  
} 

void sound_delay(uchar sound_signalhigh,uchar sound_signallow)//产生一定频率的音调 
{  
 TH1=sound_signalhigh; 
 TL1=sound_signallow; 
 TR1=1; 
 while(TF1==0); 
 TR1=0; 
 TF1=0;   
} 

void main() 
{ 
 uint k=0; 
 uchar sound_signalhigh,sound_signallow;//定时器的高8位与低8位 
 uchar sound_pace;//定义节拍计数器  
 init(); 
 while(1) 
   { 
      k=0; 
   while(tune[k]!=0x0000)//因为歌曲的结尾处人为的加入了 结束标示符  0  
   {
	sound_signallow=tune[k];
    sound_signalhigh=(tune[k]>>8);      
    //从表中取得 节拍大小  
    sound_pace=40*duration[k];//400为一拍的时长 ,在此修改一拍的时长。 
	k++;
    TR0=1; 
   //当节拍数未达到时候，继续循环，产生该音调的声音  
   while(counter!=sound_pace) 
      {  
       sound=~sound; 
       sound_delay(sound_signalhigh,sound_signallow); 
      } 
     counter=0;//节拍计数器置0，进入下一个音调  
     TR0=0; 
    } 
   delay(100); //歌曲演唱完毕后,延时一段时间  
 } 
}



 