#ifndef TWI_8
#define TWI_8
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/twi.h>

void TWIinit(void);
uint8_t TWIStart();
uint8_t TWISendDeviceAdd(unsigned char address);
void TWISend(unsigned char data);
void TWIStop();
uint8_t TWIReadAck(uint8_t daddress, uint8_t regadd);
uint8_t TWIReadNack(uint8_t daddress, uint8_t regadd);
void TWITransmit(uint8_t daddress, uint8_t regadd,uint8_t data);


#endif