#include <reg52.h>
#include "Serial.h"
#include "DHT11.h"

void main()
{
	SerialInit();
	while(1)
	{ 
	    RH();
		SerialPutString(buff);
	}

}
