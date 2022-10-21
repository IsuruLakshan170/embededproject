#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
                                                                                                                                            
void calibrateMpu6050()                                                                                                                           
{                                                                                                                                                 
  int max_samples = 2000;                                                                                                                         

  for (int i = 0; i < max_samples; i++) {                                                                                                         
   // readSensor();                                                                                                                                 

   // gyro_offset[X] += gyro_raw[X];                                                                                                                
   // gyro_offset[Y] += gyro_raw[Y];                                                                                                                
   // gyro_offset[Z] += gyro_raw[Z];                                                                                                                

    // Just wait a bit before next loop                                                                                                           
    //delay(3);                                                                                                                                     
  }                                                                                                                                               

  // Calculate average offsets                                                                                                                    
 // gyro_offset[X] /= max_samples;                                                                                                                  
  //gyro_offset[Y] /= max_samples;                                                                                                                  
  //gyro_offset[Z] /= max_samples;                                                                                                                  
} 