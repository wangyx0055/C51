#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P2^6;
sbit wela=P2^7;
sbit key1=P3^4;
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71,0};
uchar num,temp,num1;
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
uchar keyscan();
void display(uchar aa);
void main()
{
	num=17;
	dula=1;
	P0=0;
	dula=0;
	wela=1;
	P0=0xc0;
	wela=0;
	
	while(1)
		{
			display(keyscan());
		}
}
void display(uchar aa)
{
			dula=1;
			P0=table[aa-1];
			dula=0;		
}
uchar keyscan()
{
			P3=0xfe;
			temp=P3;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delay(5);
					temp=P3;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
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
					while(temp!=0xf0)
						{
							temp=P3;
							temp=temp&0xf0;
						}
					}
				}

			P3=0xfd;
			temp=P3;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delay(5);
					temp=P3;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=P3;
					switch(temp)
						{
							case 0xed:num=5;
								break;
							case 0xdd:num=6;
								break;
							case 0xbd:num=7;
								break;
							case 0x7d:num=8;
								break;
						}
					while(temp!=0xf0)
						{
							temp=P3;
							temp=temp&0xf0;
						}
					}
				}


			P3=0xfb;
			temp=P3;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delay(5);
					temp=P3;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=P3;
					switch(temp)
						{
							case 0xeb:num=9;
								break;
							case 0xdb:num=10;
								break;
							case 0xbb:num=11;
								break;
							case 0x7b:num=12;
								break;
						}
					while(temp!=0xf0)
						{
							temp=P3;
							temp=temp&0xf0;
						}
					}
				}


			P3=0xf7;
			temp=P3;
			temp=temp&0xf0;
			while(temp!=0xf0)
				{
					delay(5);
					temp=P3;
					temp=temp&0xf0;
					while(temp!=0xf0)
					{
						temp=P3;
					switch(temp)
						{
							case 0xe7:num=13;
								break;
							case 0xd7:num=14;
								break;
							case 0xb7:num=15;
								break;
							case 0x77:num=16;
								break;
						}
					while(temp!=0xf0)
						{
							temp=P3;
							temp=temp&0xf0;
						}
					}
				}
return num;

}

