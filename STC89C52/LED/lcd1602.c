#include<reg52.h>
#include <string.h>
#include "LCD.h"

unsigned char code table[]="I love China!";
unsigned char i,len;
void main()
{	

	LCDInit();
	P1=0x00;
	len=strlen(table);
	for(i=0;i<len;i++)
		LCDWriteData(table[i]);
	while(1);
}

