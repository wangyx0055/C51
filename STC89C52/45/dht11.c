#include <reg52.h>
#define uchar unsigned char
sbit dht11=P2^0;
uchar data_byte,RH,RL,TH,TL;

void delay(int x)            //delay
{
    uchar a,b;
    for(a=x;a>0;a--)
      for(b=60;b>0;b--);
}
void delay_1(int x)
{
	uchar i;
	i=2*x;
	while(i--);
}

void start()//DHT11湿度开始信号
{
    dht11=1;
    delay(1);
    dht11=0;
    delay(30);//>18ms
    dht11=1;
    delay_1(10);//20-40us
}

uchar receive_byte()
{
    uchar i,tmp,count;
    for(i=0;i<8;i++)             //循环8次直至读取所有数据（8bit）
    {
        count=2;
        while((!dht11)&&count++)//等待50us低电平结束
        tmp=0;                    // 默认将tmp置0
        delay_1(20);            //70us对应“1”，26~28us对应“0”
        if(dht11==1)tmp=1;
        count=2;
        while((dht11)&&count++);   //等待直到低电平
        if(count==1)break;
        data_byte = data_byte<<1;     //左移一位
        data_byte|=tmp;                 //赋新读取到的值，合并
    }
    return data_byte;           
}

void receive()
{
    uchar T_H,T_L,R_H,R_L,check,num_check;
    uchar count;
    start();//开始信号
    dht11=1;
    if(!dht11)//读取DHT11响应信号
    {
    count=2;
    while((!dht11)&&count++);//DHT11高电平80us是否结束
    count=2;
    while((dht11)&&count++);
    R_H=receive_byte();
    R_L=receive_byte();
    T_H=receive_byte();
    T_L=receive_byte();
    check=receive_byte();  //接收校验的8位
    dht11=0;//拉低延时50us
    delay_1(20);
    dht11=1;
    num_check=R_H+R_L+T_H+T_L;
    if(num_check==check)
    {
        RH=R_H;
		P1=RH;
        RL=R_L;
        TH=T_H;
        TL=T_L;
        check=num_check;
    }
    }
}
void main()
{
	receive();
	delay(5000);
}
