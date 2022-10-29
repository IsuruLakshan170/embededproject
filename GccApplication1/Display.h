#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h> //to use abs() function


#define LcdDataBus PORTA
#define LcdControlBus PORTA
#define LcdDataBusDirnReg DDRA

#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

volatile int ADC_value_x_prev = 0; //to check sensor value has changed
volatile int ADC_value_x = 0; //to store sensor value
volatile int calibration = 0; //store the reference angle
volatile int timeCount = 0; //timer variable
volatile int calibrated = 0; //check the calibration button is pressed or not
volatile int sleep = 0; //to check the device is in sleep or not


void initPorts (void){
	
	LcdDataBusDirnReg = 0xFF; //LCD attached port as output
	DDRD &= 0B11111110;

}



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

void initADC(void)

{
	ADCSRA |= (1<<ADEN);//Enable ADC
	ADCSRA |= 0B00000111;//Prescalar set to128
	ADCSRA |= (1<<ADSC);//Start the conversion
}


void Lcd_init(void){
	Lcd_CmdWrite(0x02); //Initialize Lcd in 4-bit mode
	Lcd_CmdWrite(0x28); //enable 5x7 mode for chars
	Lcd_CmdWrite(0x0E); //Display OFF, Cursor ON
	Lcd_CmdWrite(0x01); //Clear Display
	Lcd_CmdWrite(0x80); //Move the cursor to beginning of first line
}

int readADC(uint8_t adc_input_pin){
	ADMUX = (ADMUX & 0xF0) | adc_input_pin;
	ADMUX &= 0X0B00111111;
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC));
	
	return ADC;
}



void enableTimerInterrupt1(void)
{
	TIMSK1 |= (1<<OCIE1A);	 // Timer 1 compare interrupt 1 enable
}

void disableTimerInterrupt1(void)
{
	TIMSK1 &= ~(1<<OCIE1A);	 // Timer 1 compare interrupt 1 disable
}


void intInteruptPort0(void){
	//	EIMSK |= 0x01; //Bit0 of EIMSK set to 1(enable INT0 interrupt
	EICRA |= 0B00000011; // Trigger at rising edge
	EIMSK |= 0B00000001; // Enable External interrupt 0
}

void initTimerInterrupt1(void)
{
	TCNT1 = 0x00;		// Init counter to 0
	//Prescalar to 1024, Mode to CTC
	//TCCR1A no change
	TCCR1B |= ((1<<WGM12)|(1<<CS12)|(1<<CS10)); //Set prescaler 0 to 1
	OCR1A = 46875 - 1;
	enableTimerInterrupt1();
}



int main(void)
{

	
	
	/* Replace with your application code */

	//initialize the device
	initPorts ();
	Lcd_init();
	initADC( );
	initTimerInterrupt1();
	intInteruptPort0();
	sei();

	
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
	
	int digit1s;
	int digit10s;
	
	
	
	while (1)
	{
		
		ADC_value_x =readADC(0);//read sensor value

		
		
		
		
		if(timeCount>20 && sleep == 0)//timer condition for sleep mode
		{
			Lcd_CmdWrite(0x08);//sleep the l
			sleep = 1;
		}
		
		if(ADC_value_x_prev!=ADC_value_x){ //check the sensor value changed or not
			timeCount = 0;//reset timer

			if(sleep == 1 || calibrated == 1){ //check that device is coming from sleep mode or after calibration
				
				Lcd_init();
				
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

				calibrated = 0;
				sleep = 0;
			}
			
			
			
			
			ADC_value_x_prev = ADC_value_x;

			int x = 0;
			x = abs(ADC_value_x-calibration)%91; //get absolute value between current angle and reference angle
			
			digit1s = (x)%10;
			digit10s= (x%100)/10;
			
			
			Lcd_CmdWrite(0xC0);
			
			Lcd_DataWrite(0X30+digit10s);
			Lcd_DataWrite(0X30+digit1s);
			Lcd_DataWrite('°');
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	//tasks done in timer interrupt

	timeCount++;			// Increment the timer
	
}

ISR (INT0_vect)
{
	//tasks done when calibration button is pressed
	
	
	if (sleep==1){
		Lcd_init();
	}
	
	else{
		Lcd_CmdWrite(0x01);
		Lcd_CmdWrite(0x80);
	}
	
	
	Lcd_DataWrite('C');
	Lcd_DataWrite('a');
	Lcd_DataWrite('l');
	Lcd_DataWrite('i');
	Lcd_DataWrite('b');
	Lcd_DataWrite('r');
	Lcd_DataWrite('a');
	Lcd_DataWrite('t');
	Lcd_DataWrite('e');
	Lcd_DataWrite('d');

	Lcd_CmdWrite(0xC0);
	Lcd_DataWrite(0X30+0);
	Lcd_DataWrite(0X30+0);
	Lcd_DataWrite('°');
	
	calibration = ADC_value_x; //store reference angle
	calibrated =1;
	sleep=0;
	timeCount = 0; //reset timer

}