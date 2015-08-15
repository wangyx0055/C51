#include<reg52.h>
#include"ad.h"
#define uchar unsigned char

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
void main()
{
	while(1)
	{
		ad();
	}
}
