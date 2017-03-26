/*
 * TWI Library file
 * 50KHz, prescaler 1
 * Created: 1/29/2017 1:22:56 PM
 * Author : Abhinav
 */ 

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/twi.h>
#include "TWI_8.h"

void TWIinit(void)
{	
	TWBR=2; //Clock = 50KHz
	TWCR = (1<<TWINT)|(1<<TWEN); //Enable TWI
}

uint8_t TWIStart()
{ 
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA); //Send Start
	while(!(TWCR & (1<<TWINT))); //Wait for start to be transmitted
	
	if ((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}
	return 0;
	
}
uint8_t TWISendDeviceAdd(unsigned char address)
{
	TWISend(address);
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
	{
		return 1;
	}
	
	return 0;
}

void TWISend(unsigned char data)
{
	TWDR = data;
	TWCR =(1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	//if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
	//{
	//return 1;
	//}
}



void TWIStop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);//stop
}

uint8_t TWIReadAck(uint8_t daddress,uint8_t regadd/*, uint8_t bytes*/)
{
	/*uint8_t * point;
	uint8_t data[bytes];
	point=data;*/
	TWIStart();
	TWISendDeviceAdd(daddress);
	TWISend(regadd);
	TWIStart();
	TWISendDeviceAdd(daddress+1);
	/*for(int i =0; i <bytes;i++)
	{
		data[i]= TWDR;
		//send ack
	}*/
	uint8_t data = TWDR;
	TWCR = (1<<TWEA)|(1<<TWINT);
	TWIStop();
	return data;
	
}

uint8_t TWIReadNack(uint8_t daddress, uint8_t regadd)
{
	TWIStart();
	TWISendDeviceAdd((daddress<<1));
	TWISend(regadd);
	TWIStart();
	TWISendDeviceAdd((daddress<<1)+1);
	TWCR = (1<<TWINT | 1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	uint8_t data=TWDR;
	TWIStop();
	return data;
}

void TWITransmit(uint8_t daddress, uint8_t regadd,uint8_t data)
{
	TWIStart();
	TWISendDeviceAdd(daddress<<1);
	TWISend(regadd);
	TWISend(data);
	
	TWIStop();
}
