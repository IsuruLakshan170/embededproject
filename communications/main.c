

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "IncFile1.h" //i2c

#define F_CPU 16000000 //clock speed 
#define BUAD 9600 //baud rate 
#define BRC ((F_CPU/16/BUAD) - 1)//baud rate calculate
#define TX_BUFFER_SIZE 128


char serialBuffer[TX_BUFFER_SIZE];
uint8_t serialReadPos = 0;
uint8_t serialWritePos = 0;

void appendSerial(char c);
void serialWrite(char c[]);

int main(void)
{
    UBRR0H = (BRC >> 8); //upper byte
	UBRR0L = BRC;//lower byte
	
	UCSR0B = (1 << TXEN0 ) | (1 << TXCIE0);//transmitter enable
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);//8 bit data frame
    
 	sei();
	//serialWrite("Hello\n\r");
	//serialWrite("World\n\r");

/*	
	DDRB = 0xFF;
	DDRA = 0xFF;		//--- Defining PORT A as output
	i2c_init();			//--- I2C Initialization
	
*/	
	while (1) 
    {
		UDR0 ='8';
	    _delay_ms(1000);
		/*
		
		i2c_start();		//--- Start condition to slave
		i2c_write(0x41);	//--- Write Slave address to enable it
		PORTA = i2c_read();	//--- Move data in IO Pin to PORTA
		
		*/
    }
}

void appendSerial(char c){
	serialBuffer[serialWritePos] = c;
	serialWritePos ++;
	
	if(serialWritePos >= TX_BUFFER_SIZE){
		serialWritePos = 0;
	}
}
void serialWrite(char c[])
{
	for(uint8_t i =0 ;i< strlen(c);i++){
		appendSerial(c[i]);
		
	}
	if(UCSR0A & (1<< UDRE0))
	{
		UDR0 = 0;
		
	}
	serialBuffer[serialWritePos];
}

ISR(USART0_RX_vect)
{
	if(serialReadPos != serialWritePos)
	{
		
		UDR0 = serialBuffer[serialReadPos];
		serialReadPos++;
		
		if(serialReadPos >= TX_BUFFER_SIZE)
		{
			
			serialReadPos ++;
		}
	}
}




