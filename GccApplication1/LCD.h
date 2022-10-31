#ifndef LCD_H_				
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#define LcdDataBus PORTA
#define LcdControlBus PORTA
#define LcdDataBusDirnReg DDRA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void Lcd_CmdWrite(char cmd){
	LcdDataBus = (cmd & 0xF0); //Set upper 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS) ; //Set RS pin to LOW
	LcdControlBus &= ~(1<<LCD_RW); //Set RW pin to LOW

	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms (10) ;
	LcdControlBus &= ~(1<<LCD_EN);

	_delay_ms (10) ;
	LcdDataBus = ((cmd<<4) & 0xF0); //Set lower 4 bits of the cmd
	LcdControlBus &= ~(1<<LCD_RS) ; //Set RS pin to LOW

	LcdControlBus &= ~(1<<LCD_RW) ; //Set RW pin to LOW
	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms (10) ;
	LcdControlBus &= ~(1<<LCD_EN);
	_delay_ms (10) ;
}

void Lcd_init(void){
	LcdDataBusDirnReg = 0xFF; //LCD attached port as output
	Lcd_CmdWrite(0x02); //Initialize Lcd in 4-bit mode
	Lcd_CmdWrite(0x28); //enable 5x7 mode for chars
	Lcd_CmdWrite(0x0E); //Display OFF, Cursor ON
	Lcd_CmdWrite(0x01); //Clear Display
	Lcd_CmdWrite(0x80); //Move the cursor to beginning of first line
}

void Lcd_DataWrite(char dat){
	LcdDataBus = (dat & 0xF0);//Set upper 4 bits of the data
	LcdControlBus |= (1<<LCD_RS) ; //Set RS pin to High
	LcdControlBus &= ~(1<<LCD_RW); //Set RW pin to LOW

	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms (10);
	LcdControlBus &= ~(1<<LCD_EN);

	_delay_ms(10) ;
	LcdDataBus = ((dat<<4) & 0xF0);//Set lower 4 bits of the data
	LcdControlBus |= (1<<LCD_RS); //Set RS pin to HIGH
	LcdControlBus &= ~(1<<LCD_RW) ; //Set RW pin to LOW

	LcdControlBus |= (1<<LCD_EN); //Generate High-to-Low pulse
	_delay_ms (10);
	LcdControlBus &= ~(1<<LCD_EN);
	_delay_ms (10) ;
}

void SendLCD(int angle, int refangle){
	dtostrf( pitch, 3, 2, float_ );
	sprintf(buffer,"Pitch = %s\n",float_);
	
	Lcd_DataWrite('A');
	Lcd_DataWrite('n');
	Lcd_DataWrite('g');
	Lcd_DataWrite('l');
	Lcd_DataWrite('e');
	Lcd_DataWrite(' ');
	Lcd_DataWrite('i');
	Lcd_DataWrite('n');
	Lcd_DataWrite(' ');
	Lcd_DataWrite('d');
	Lcd_DataWrite('e');
	Lcd_DataWrite('g');
	
	Lcd_CmdWrite(0xC0);
}

#endif	