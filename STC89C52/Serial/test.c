#include <string.h>
#include "Serial.h"

unsigned char buff1[32];
unsigned char val;
void main()
{
	SerialInit();
	memset(buff1,0x00,32);
	SerialPutString("hello world!");
	while(1)
	{
		if(SerailRevCheck()>0)
		{
			//val=SerialGetDataLen();
			SerialRevData(buff1);
			SerialPutString(buff1);
			val=SerialGetDataLen();
			val+=48;
			SerialPutString(&val);
			P1=0x00;
		}
	}		
}
