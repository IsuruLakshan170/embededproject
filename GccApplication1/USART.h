
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

#include <avr/io.h>
#include <util/delay.h>

unsigned char rxByte;

void initUSART(void)
{
	UBRR0H =(unsigned char)(UBRR_VALUE>>8);
	UBRR0L =(unsigned char) (UBRR_VALUE);
	UCSR0B =(1<<RXEN0) | (1<<TXEN0);
	UCSR0C =(3<<UCSZ00);

}

void transmitUSART(unsigned char data)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 =data;
}

unsigned char receiveUSART(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0; 
}

void USART_SendString(char *str){						/* Send string of USART data function */
	int i=0;
	while (str[i]!=0){
		transmitUSART(str[i]);							/* Send each char of string till the NULL */
		i++;
	}
	return;
}