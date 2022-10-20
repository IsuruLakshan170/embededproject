/* Header File for I2C */

#include <avr/io.h>			//--- Reg file of ATmega16

#define F_CPU	16000000UL	//--- CPU Clock Frequency

//--- I2C Initialize ---//

void i2c_init()
{
	TWBR = 0x62;		//--- Baud rate is set by calculating the of TWBR see Details for Reference
	TWCR = (1<<TWEN);	//--- Enable I2C
	TWSR = 0x00;		//--- Prescaler set to 1
}

//--- I2C Start Condition ---//

void i2c_start()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	//--- Start Conditon
	while (!(TWCR & (1<<TWINT)));				//--- Check for Start condition successful
}

//--- I2C Stop Condition ---//

void i2c_write(char x)
{
	TWDR = x;						//--- Move value to I2C reg
	TWCR = (1<<TWINT) | (1<<TWEN);	//--- Enable I2C and Clear Interrupt
	while (!(TWCR & (1<<TWINT)));	//--- Write Successful with TWDR Empty
}

//--- I2C Read Condition ---//

char i2c_read()
{
	TWCR = (1<<TWEN) | (1<<TWINT);	//--- Enable I2C and Clear Interrupt
	while (!(TWCR & (1<<TWINT)));	//--- Read successful with all data received in TWDR
	return TWDR;
}

//--- End of Header ---//