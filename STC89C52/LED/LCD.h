#define uchar unsigned char
#define uint unsigned int
sbit  wei=P2^6;
sbit  duan=P2^7;
sbit lcden=P3^4;
sbit lcdrs=P3^5;

void LCDDelay(int n)
{
	int x,y;
	for(x=n;x>0;x--)
		for(y=110;y>0;y--);
}

void LCDWriteCom(uchar com)
{
	lcdrs=0;
	P0=com;
	LCDDelay(1);
	lcden=1;
	LCDDelay(5);
	lcden=0;
}
void LCDWriteData(uchar value)
{
	lcdrs=1;
	P0=value;
	LCDDelay(1);
	lcden=1;
	LCDDelay(5);
	lcden=0;
}

void LCDInit()
{
	duan=0;
	wei=0;
	lcden=0;
	//count=0;
	LCDWriteCom(0x38);
	LCDWriteCom(0x0c);
	LCDWriteCom(0x06);
	LCDWriteCom(0x01);//«Â∆¡
	LCDWriteCom(0x80);
}