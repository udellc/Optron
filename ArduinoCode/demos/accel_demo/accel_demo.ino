//==========================================================
// ACCEL DEMO
// Connor Hodgert
//==========================================================
#include "MyMPU6050.h"

// defines:
#define INTERVAL        100

#define SERIAL_BAUD_RATE  19200

// globals:
unsigned long previous_millis;
unsigned long current_millis;
MyMPU6050 mpu;

// functions:

void setup()
{
  // mpu
  Wire.begin();
  mpu.initialize( );
  // serial
  Serial.begin( SERIAL_BAUD_RATE );
  // timing
  previous_millis = 0;
  current_millis = millis( );
  
  // notify
  Serial.println( "SETUP COMPLETE" );
}

void loop() 
{
  current_millis = millis( );
  if( (current_millis - previous_millis) < INTERVAL ) return;
  previous_millis = current_millis;

  mpu.get_gyro( );
  mpu.get_accel( );
  mpu.print_data( );
}
