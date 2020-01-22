/* ******************************************************
 * Includes
 * ******************************************************/
 #include <MPU6050.h>

/* ******************************************************
 * Functions
 * ******************************************************/
 // initialize
void MPU6050::init( )
{
  // TODO
  deg_x = 0.0f;
  deg_y = 0.0f;
  deg_z = 0.0f;
  accel_x = 0.0f;
  accel_y = 0.0f;
  accel_z = 0.0f;
}

// update degrees
void MPU6050::get_degrees( )
{
  // TODO
  deg_x = 0.0f;
  deg_y = 0.0f;
  deg_z = 0.0f;
}

// update acceleration
void MPU6050::get_acceleration( )
{
  // TODO
  accel_x = 0.0f;
  accel_y = 0.0f;
  accel_z = 0.0f;
}

// transmit to MaxMSP
void MPU6050::transmit_data( )
{
  // TODO
  Serial.print( "%f%f%f\n", deg_x, deg_y, deg_z );
  Serial.print( "%f%f%f\n", accel_x, accel_y, accel_z );
}
