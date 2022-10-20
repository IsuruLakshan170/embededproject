#include <avr/io.h>
#include "I2C.h"
#define F_CPU 16000000
#define BUAD 9600
#define BUAD_RATE_CALC ((F_CPU/16/BUAD) - 1)

int main(void){
	
	i2c_init();			//--- I2C Initialization
		
	char ar[32];
	
    //High and low bits
    UBRR0H = (BUAD_RATE_CALC >> 8);
    UBRR0L = BUAD_RATE_CALC;
    ////////////////

    UCSR0B = (1 << TXEN0)| (1 << TXCIE0) | (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  //8 bit data format
    ////////////////////////////////////////////////////////////////

    while (1){
		
		i2c_start();		//--- Start condition to slave
		i2c_write(0x41);	//--- Write Slave address to enable it
	//	PORTA = i2c_read();	//--- Move data in IO Pin to PORTA
		 char arr[32] = i2c_read();
		 
		//unlimited processing
		for (int i = 0; i < strlen(arr); i++){
			while (( UCSR0A & (1<<UDRE0))  == 0){};
			UDR0 = arr[i];
		}
		
    }
}





