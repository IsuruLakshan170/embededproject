#define F_CPU 16000000UL								
#include <avr/io.h>										
#include <util/delay.h>									
#include <inttypes.h>									
#include <avr/interrupt.h>
#include <stdlib.h>										
#include <stdio.h>										
#include <util/delay.h>
#include <math.h>
#include "MPU6050.h"									/* Include MPU6050 registers list file */
#include "I2C.h"										/* Include I2C Functions */
#include "USART.h"										/* Include USART Functions */
#include "AngleRawData.h"								/*Include MPU6050 Functions */
#include "LCD.h"										/*Include LCD Display Functions */
#include "Interrupt.h"									/*Include Interrupt Functions */

#define A 0.962
#define dt 0.020
#define PI 3.14159

int pitchangle;
int pitch;

float Xa = 0,Ya = 0,Za = 0;
float Xg=0,Yg=0;
int ref_angle= 0;
int timeCounter = 0;

int main(){
	DDRB |= 0xFF;
	DDRE &= 0x00;//input
	//system initializations
	I2C_Init();
	MPU6050_Init();
	initUSART();
	Lcd_init();
	//Timer interrupt initializations 
	initTimerInterrupt1();
	//Extranal interrupt initializations
	intInteruptPort();
	//Enabel Global intrrupt	
	sei();
	PORTB |= 0xFF;
	while(1){
		
		//get data from Sensor using AngleRowData lib
		float* dataArray = Read_RawValue();
		//read row data
		Xa = dataArray[0]/16384.0;
		Ya = dataArray[1]/16384.0;
		Za = dataArray[2]/16384.0;
		Xg = dataArray[3]/16.4;
		Yg = dataArray[4]/16.4;
		
		//calculate pitch angle
		pitchangle=atan2(Xa,sqrt(Ya*Ya+Za*Za))*240/PI;
		
		//Calculate  pitch
		pitch = A*(pitch+Yg*dt)+(1-A)*pitchangle;
		
		//send output
		SendOut(pitch,ref_angle);
		
		//check sleep
		if(timeCounter >= 10){
			Lcd_CmdWrite(0x08);
		}
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){
	if(prvAngle != defAngle){
		timeCounter = 0;
	}
	LCDDisplay();
	timeCounter++;
}

ISR(INT4_vect){
	ref_angle = pitch;	
}
