#include <intrins.h>
unsigned char a,A1,A2,A2t,A3;
sbit adrd=P3^7;                          //IO口定义
sbit adwr=P3^6;
sbit diola=P2^5;
sbit dula=P2^6;
sbit wela=P2^7;
sbit led_stop=P2^5;
unsigned char j,k,adval;
extern void delay(unsigned char i);

	void ad()		//	主程序
	{
		dula=0;
		dula=1;
		P0=0x00;
		dula=0;
		
		led_stop=0;
		wela=1;
	    P0=0;                       //选通ADCS
	    adwr=0;			//AD写入（随便写个什么都行，主要是为了启动AD转换）
	    _nop_();
	    adwr=1;
	    P0=0xff;                    //关闭ADCS
	    delay(10);
	    wela=0;                     //关闭有AD片选信号锁存器的锁存端以防止在操作数码管时使AD的片选发生变化
	    delay(50);                 //需要注意的是ADC0804在写和读之间的时间间隔要足够长否则无法读出数据
	                                //这里把显示部分放这里的原因也是为了增加写读之间的时间间隔                                                                                                                                                                                                                           
	  								//送去显示各位。
		wela=1;                    //重新打开有AD片选信号锁存器的锁存端
	    P1=0xff;                    //读取P1口之前先给其写全1
	    P0=0;                       //选通ADCS
	    adrd=0;                     //AD读使能
	    adval=P1;			//AD数据读取赋给P1口
	    adrd=1;
	    P0=0xff;                     //关闭ADCS
	    adwr=0;
//	    P1=~adval;  /同时把AD的值送八个发光二极显示


	}
