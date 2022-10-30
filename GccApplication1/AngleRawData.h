
#ifndef ANGLE_RAW_H_					/* Define library H file if not defined */
#define ANGLE_RAW_H_

#include "MPU6050.h"									/* Include MPU6050 register define file */
#include "I2C.h"										/* Include I2C Functions */

float dataSet[6];

void MPU6050_Init(){									/* Gyro initialization function */
	_delay_ms(150);										/* Power up time >100ms */
	I2C_Start_Wait(0xD0);								/* Start with device write address */
	I2C_Write(SMPLRT_DIV);								/* Write to sample rate register */
	I2C_Write(0x07);									/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(PWR_MGMT_1);								/* Write to power management register */
	I2C_Write(0x01);									/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(CONFIG);									/* Write to Configuration register */
	I2C_Write(0x00);
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(GYRO_CONFIG);								/* Write to Gyro configuration register */
	I2C_Write(0x18);									/* Full scale range +/- 2000 degree/C */
	I2C_Stop();

	I2C_Start_Wait(0xD0);
	I2C_Write(INT_ENABLE);								/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(0xD0);								/* I2C start with device write address */
	I2C_Write(ACCEL_XOUT_H);							/* Write start location address from where to read */
	I2C_Repeated_Start(0xD1);							/* I2C start with device read address */
}

float* Read_RawValue(){									/* Read Gyro values */
	
	MPU_Start_Loc();
	dataSet[0] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack()); // Acc_x
	dataSet[1] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack()); //Acc_y
	dataSet[2] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack()); //Acc_z
	dataSet[3] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack()); // Gyro_x
	dataSet[4] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack()); //Gyro_y
	dataSet[5] = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack()); // Gyro_z
	I2C_Stop();
	return dataSet;
}

#endif	