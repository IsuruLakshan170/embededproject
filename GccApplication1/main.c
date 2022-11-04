#define F_CPU 16000000UL								/* Define CPU clock Frequency e.g. here its 16MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <avr/interrupt.h>
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include <util/delay.h>
#include <math.h>
#include "MPU6050.h"									/* Include MPU6050 register define file */
#include "I2C.h"										/* Include I2C Functions */
#include "USART.h"										/* Include USART Functions */
#include "AngleRawData.h"								/*Include MPU6050 Functions */
#include "LCD.h"										/*Include LCD Display Functions */


#define A 0.962
#define dt 0.020
#define PI 3.14159

int rollangle,pitchangle;
int roll,pitch,yaw;

float Xa = 0,Ya = 0,Za = 0;
float Xg=0,Yg=0;
volatile int ref_angle=0;

void enableTimerInterrupt1(void)
{
	TIMSK1 |= (1<<OCIE1A);	 // Timer 1 compare interrupt 1 enable
}

void disableTimerInterrupt1(void)
{
	TIMSK1 &= ~(1<<OCIE1A);	 // Timer 1 compare interrupt 1 disable
}

void initTimerInterrupt1(void)
{
	TCNT1 = 0x00;		// Init counter to 0
	TCCR1B |= ((1<<WGM12)|(1<<CS12)|(1<<CS10)); //Set prescaler 0 to 1
	OCR1A = 46875 - 1;
	enableTimerInterrupt1();
}

void intInteruptPort(void){
	
	EICRA |= 0B00000011;
	EIMSK |= 0B00010000; // Enable External interrupt 4
}

int main(){
	
	//system initializations
	I2C_Init();
	MPU6050_Init();
	initUSART();
	Lcd_init();
	//Timer interrupt initializations 
	initTimerInterrupt1();
	//Extranal interrupt initializations
	intInteruptPort();
	//Enabel extranel intrrupt
	sei();	

	while(1){
		
		//get data from Sensor using AngleRowData lib
		float* dataArray = Read_RawValue();
		
		
		//read row data
		Xa = dataArray[0]/16384.0;
		Ya = dataArray[1]/16384.0;
		Za = dataArray[2]/16384.0;
		Xg = dataArray[3]/16.4;
		Yg = dataArray[4]/16.4;
		
		//calculate pitch
		pitchangle=atan2(Xa,sqrt(Ya*Ya+Za*Za))*240/PI;
		
		//Calculate  pitch
		pitch=A*(pitch+Yg*dt)+(1-A)*pitchangle;
		//send output
		SendOut(pitch,0);
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){
	LCDDisplay();
}

ISR(INT4_vect){
	ref_angle = pitch;	
}
