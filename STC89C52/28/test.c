#include <reg52.h>
#define uchar unsigned char
sbit duan=P2^6;
sbit wei=P2^7;
sbit lcden=P3^4;
sbit rs=P3^5;
sbit s1=P3^1;
sbit s2=P3^2;
sbit s3=P3^3;
sbit rd=P3^7;
uchar code table[]="2013-10-26 Sat";
uchar code table1[]="00:00:00";
void init();
void writeCom(uchar com);
void writeData(uchar value);
void writeTime(uchar add,uchar time);
void delay(uchar z);
uchar i,count,shi,fen,miao,s1num;
void main()
{
	init();
	for(i=0;i<14;i++)
		writeData(table[i]);
	writeCom(0x80+0x40+0x05);
	for(i=0;i<8;i++)
		writeData(table1[i]);
	while(1)
	{
		if(count==18)
		{
			count=0;
			miao++;
			if(miao==60)
			{
				miao=0;
				fen++;
				if(fen==60)
				{
					fen=0;
					shi++;
					if(shi==24)
						shi=0;
					writeTime(5,shi);
				}
				writeTime(8,fen);
			}
			writeTime(0x0b,miao);
		}
		if(s1==0)
		{
			delay(5);
			if(s1==0)
			{
				TR0=0;
				while(!s1);
				delay(5);
				s1num++;
				s1num=s1num%4;
				switch(s1num)
				{
				case 1:writeCom(0x80+0x40+0x0c);break;
				case 2:writeCom(0x80+0x40+9);break;
				case 3:writeCom(0x80+0x40+6);break;
				}
			}
		}
		//
		if(s1num==0)
		{
			TR0=1;	
			writeCom(0x0c);
		}
		else
			writeCom(0x0d);
		//
		if(s2==0)
		{
			delay(5);
			if(s2==0)
			{
				while(!s2);
				delay(5);			
				switch(s1num)
				{
					case 1:miao++;miao=miao%60;writeTime(0x0b,miao);writeCom(0x80+0x40+0x0c);break;
					case 2:fen++;fen=fen%60;writeTime(8,fen);writeCom(0x80+0x40+9);break;
					case 3:shi++;shi=shi%24;writeTime(5,shi);writeCom(0x80+0x40+6);break;
				}				
			}
		}
		//
		if(s3==0)
		{
			delay(5);
			if(s3==0)
			{
				while(!s3);
				delay(5);			
				switch(s1num)
				{
					case 1:if(miao==0)
				 				miao=59;
							else
								miao--;
							writeTime(0x0b,miao);
							writeCom(0x80+0x40+0x0c);break;
					case 2:if(fen==0)
				 				fen=59;
							else
								fen--;
							writeTime(8,fen);
							writeCom(0x80+0x40+9);break;
					case 3:if(shi==0)
				 				shi=23;
							else
								shi--;
							writeTime(5,shi);
							writeCom(0x80+0x40+6);break;
				}				
			}
		}
	}
}

void writeCom(uchar com)
{
	rs=0;
	delay(5);
	lcden=1;
	P0=com;
	delay(5);
	lcden=0;
}

void writeData(uchar value)
{
	rs=1;
	delay(5);
	lcden=1;
	P0=value;
	delay(5);
	lcden=0;
}

void writeTime(uchar add,uchar time)
{
	uchar n,m;
	n=time/10;
	m=time%10;
	writeCom(0x80+0x40+add);
	writeData(0x30+n);
	writeData(0x30+m);
}
void init()
{
	shi=fen=miao=0;
	count=0;
	duan=0;
	wei=0;
	rd=0;
	s1num=0;
	//
	writeCom(0x38);
	writeCom(0x0c);
	writeCom(0x06);
	writeCom(0x01);
	writeCom(0x80);
	//
	TMOD=0x01;
	TH0=(65536-50000)/128;
	TL0=(65536-50000)%128;
	EA=1;
	ET0=1;
	TR0=1;
}

void delay(uchar z)
{
	int x,y;
	for(x=z;x>0;x--)
		for(y=0;y<110;y++);
}

void timer0() interrupt 1
{
	TH0=(65536-50000)/128;
	TL0=(65536-50000)%128;
	count++;
}
