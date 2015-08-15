#include <reg52.h>
unsigned char code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
sbit wei=P2^7;
sbit duan=P2^6;
unsigned char temp,temp1,test,num;
void display(unsigned char num);
void delay1();
unsigned char getIndex(unsigned char x,unsigned char y);
unsigned char getValue(unsigned char y);
//将行线所接的单片机的I/O口作为输出端，而列线所接的I/O口则作为输入
void main()
{
	wei=1;
	P0=0xfc;
	wei=0;

	duan=1;
	P0=0;
	duan=0;
	while(1)
	{
			P3=0xfd;
			temp=P3;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delay1();
					temp=P3;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=P3;
					switch(temp)
						{
							case 0xed:num=4;
								break;
							case 0xdd:num=5;
								break;
							case 0xbd:num=6;
								break;
							case 0x7d:num=7;
								break;
						}
					while(temp!=0xf0)
						{
							temp=P3;
							temp=temp&0xf0;
						}
					}
				}

		display(num);
		}
	/*
	while(1)
	{
		P3=0xfe;
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
			delay1();
			temp=P3;
			temp=temp&0xf0;
			if(temp!=0xf0)
			{
				//temp1=P3;
				
			//	temp1=getIndex(0,temp);
			//	display(temp1);
				
				temp=P3;
					switch(temp)
						{
							case 0xee:num=1;
								break;
							case 0xde:num=2;
								break;
							case 0xbe:num=3;
								break;
							case 0x7e:num=4;
								break;
						}

				}

			while(temp!=0xf0);
			{
				temp=P3;
				temp=temp&0xf0;
			}
			delay1();
			while(temp!=0xf0)
			{
				temp=P3;
				temp=temp&0xf0;
			}
		}
	//	P1=temp1;
	//	display(temp1);
	
	//	display(temp);
	display( num);
	}
	*/
}

void display(unsigned char num)
{
	duan=1;
	P0=table[num];
	duan=0;
}

void delay1()
{
	int i,j;
	for(i=5;i>0;i--)
		for(j=110;j>0;j--);
}

unsigned char getIndex(unsigned char x,unsigned char y)
{
	unsigned char high,m,n,value;
//	low=x&0x0f;
//	high=x&0xf0;
	high=y>>4;
//	m=getValue(low);
	m=x;
	n=getValue(high);
//	value=table[m][n];
//	display(table[m][n]);
//	delay1();
	return value;
}

unsigned char getValue(unsigned char y)
{
	unsigned char temp;
	temp=~y;
	if(temp%8==0)
		return 3;
	else if(temp%4==0)
		return 2;
	else if(temp%2==0)
		return 1;
	else 
		return 0;
}
