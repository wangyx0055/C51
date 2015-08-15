#include<reg52.h>           
#define uchar unsigned char 
#define uint unsigned int 

sbit sound=P2^3; 
uint counter=0; 

uchar code FREQH[3][8]=                    //音符定时器初值,三个8度音的高八位 
{//注意每行起始都为0xff，因为音调中没有 0 这个音 ，所以随便填了个数字，后面不不会
//引用它 
     // 低音： 
    //  1       2      3      4      5      6      7 
  {0xff,0xf9,   0xfa,  0xfa,  0xfb,  0xfb,  0xfc,  0xfc}, 
       //中音： 
      //1       2      3      4      5      6      7 
  {0xff,0xfc,   0xfd,  0xfd,  0xfd,  0xfd,  0xfe,  0xfe}, 
       //高音: 
      //1       2      3      4      5      6      7 
  {0xff,0xfe,   0xfe,  0xfe,  0xfe,  0xfe,  0xff,  0x00}//0x00 
};  

uchar code FREQL[3][8]=                    //音符定时器初值,三个8度音的低八位 
{ 
     // 低音： 
    //  1       2      3      4      5      6      7 
  {0xff,0x5b,   0x15,  0xb9,  0x04,  0x90,  0x0c,  0x79}, 
       //中音： 
      // 1      2      3      4      5      6      7 
  {0xff,0xac,   0x09,  0x5c,  0x82,  0xc8,  0x05,  0x3c}, 
       //高音: 
      //1       2      3      4      5      6      7 
  {0xff,0x55,   0x84,  0xad,  0xc0,  0xe3,  0x02,  0x00}//0x00 
}; 

uchar code MUSIC[]= 
{//生日快乐 
//歌曲代码,每三个表示一个音符及节拍，第一个表示音符1234567，第二个表示音高，0表
//示低音，1表示中音，2表示高音，第三个表示节拍长度，1表示半拍，2表示一拍，以此
//类推。 
 6,1,3,  5,1,1,  3,1,2,   5,1,2,   1,2,2,  6,1,1,  5,1,1,  6,1,4,   3,1,1,   5,1,1,  6,1,1,  
 5,1,2,  3,1,2,  
 1,1,1,  6,0,1,  5,1,1,   3,1,1,   2,1,4,  2,1,3,  3,1,1,  5,1,2,   5,1,1,   6,1,1,   
 3,1,2,  2,1,2,  1,1,4,   5,1,4,   3,1,1,  2,1,1,  1,1,1,  6,0,1,   1,1,1,   5,0,8,  0 
}; 

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
 uint i,j,k=0; 
 uchar sound_signalhigh,sound_signallow;//定时器的高8位与低8位 
 uchar sound_pace;//定义节拍计数器  
 init(); 
 while(1) 
   { 
      k=0; 
   while(MUSIC[k]!=0)//因为歌曲的结尾处人为的加入了 结束标示符  0  
   { 
    j=MUSIC[k++]; //从表中取得 音符大小  
    i=MUSIC[k++]; /*从表中取得 音符音高，也就是这个音符是低音，中音还是高
音,有了这两个参数我们就可以确定音符对应的定时器初值在表中的位置了。 */
    sound_signalhigh=FREQH[i][j]; 
    sound_signallow=FREQL[i][j]; 
    //从表中取得 节拍大小  
    sound_pace=60*MUSIC[k++];//600为一拍的时长 ,在此修改一拍的时长。 
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
   delay(100); //歌曲演唱完毕后,延时一段时间     }  
} 
}


 