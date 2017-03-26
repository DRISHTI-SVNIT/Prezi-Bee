#include "USART_32.h"

void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	//unsigned int num;
	//num=((F_CPU)/(16*baud))-1;
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN); //|(1<<RXCIE);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_InterruptEnable()
{
	UCSRB|=(1<<RXCIE);
}

void USART_Transmitchar( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}

unsigned char USART_ReceiveNonBlocking(void)
{
	if((UCSRA & (1<<RXC)))
		return UDR;
	else return 0;
}

void USART_TransmitString(char *str)
{
	 while(*str>0)
	 {
		 USART_Transmitchar(*str);
		 //_delay_ms(1);
		 str++;
	 }
}

void USART_TransmitNumber(long num){
 
   /* n is between 0 and 9 */

if(num<0)
{
	USART_Transmitchar('-');
	num=(-1)*num;
}

if(num >= 10){
	USART_TransmitNumber(num/10);
	num = num%10;
}
USART_Transmitchar(num+'0'); /* n is between 0 and 9 */

}

void USART_TransmitBinary(int num)
{
	int i=0,j=0;
	while(num)
	{
		USART_TransmitNumber(num%2);
		i++;
		num=num/2;
	}
	if(i!=8)
	{
		for(j=0;j<(8-i);j++)
		USART_TransmitNumber(0);
	}
	
}
