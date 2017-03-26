#ifndef USART_32
#define USART_32

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define New_Line() USART_Transmitchar(0x0A);\
USART_Transmitchar(0x0D);
#define Space(n) for(int i=0;i<=n;i++)USART_Transmitchar(0x20);

void USART_Init(unsigned int);
void USART_Transmitchar( unsigned char);
unsigned char USART_ReceiveNonBlocking();
unsigned char USART_Receive(void);
void USART_InterruptEnable(void);
void USART_TransmitString(char *str);
void USART_TransmitNumber(long);
void USART_TransmitBinary(int num);

#endif
