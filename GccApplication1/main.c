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
#include "Interrupt.h"									/*Include Interrupt Functions */

#define A 0.962
#define dt 0.020
#define PI 3.14159

int rollangle,pitchangle;
int roll,pitch,yaw;

float Xa = 0,Ya = 0,Za = 0;
float Xg=0,Yg=0;
int ref_angle= 0;

int main(){
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
		pitch = A*(pitch+Yg*dt)+(1-A)*pitchangle;
		SendOut(pitch,ref_angle);
	}
	return 0;
}

ISR(TIMER1_COMPA_vect){
	LCDDisplay();
}

ISR(INT4_vect){
	ref_angle = pitch;	
}
