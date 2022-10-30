#define F_CPU 16000000UL								/* Define CPU clock Frequency e.g. here its 16MHz */
#include <avr/io.h>										/* Include AVR std. library file */
#include <util/delay.h>									/* Include delay header file */
#include <inttypes.h>									/* Include integer type header file */
#include <stdlib.h>										/* Include standard library file */
#include <stdio.h>										/* Include standard library file */
#include "MPU6050.h"									/* Include MPU6050 register define file */
#include "I2C.h"										/* Include I2C Functions */
#include "USART.h"										/* Include USART Functions */
#include "AngleRowData.h"								/*Include MPU6050 Functions */

#define A 0.962
#define dt 0.020
#define PI 3.14159

int rollangle,pitchangle;
int roll,pitch,yaw;

char buffer[20], float_[10];
float Xa = 0,Ya = 0,Za = 0;
float Xg=0,Yg=0;

int main(){
	
	I2C_Init();											/* Initialize I2C */
	MPU6050_Init();										/* Initialize MPU6050 */
	initUSART();										/* Initialize USART */

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
		rollangle=atan2(Ya,Za)*180/PI;
		pitchangle=atan2(Xa,sqrt(Ya*Ya+Za*Za))*256/PI; 
  
		//Calculate roll and pitch
		roll=A*(roll+Xg*dt)+(1-A)*rollangle;//
		pitch=A*(pitch+Yg*dt)+(1-A)*pitchangle;
		
		//send data to PC
		dtostrf( pitch, 3, 2, float_ );
		sprintf(buffer,"Pitch = %s\n",float_);
		USART_SendString(buffer);
		printf("\n");
	}
}
