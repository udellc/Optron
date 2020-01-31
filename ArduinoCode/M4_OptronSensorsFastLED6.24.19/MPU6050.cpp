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
  // zero stuff out
  gyro_x = 0.0f;s
  gyro_y = 0.0f;
  gyro_z = 0.0f;
  rot_x = 0.0f;
  rot_y = 0.0f;
  rot_z = 0.0f;
  accel_x = 0.0f;
  accel_y = 0.0f;
  accel_z = 0.0f;;
  gforce_x = 0.0f;
  gforce_y = 0.0f;
  gforce_z = 0.0f;

  // init power management
  Wire.beginTransmission( MPU_ADDR );
  Wire.write( MPU_PWR_MGMT_1 );
  Wire.write( 0b00000000 );  // never sleep
  Wire.endTransmission( );
  // init gyro config to full scale
  Wire.beginTransmission( MPU_ADDR );
  Wire.write( MPU_GYRO_CONFIG );
  Wire.write( 0b00000000 );
  Wire.endTransmission( );
  // init accel to +/- 2g
  Wire.beginTransmission( MPU_ADDR );
  Wire.write( MPU_ACCEL_CONFIG );
  Wire.write( 0b00000000 );
  Wire.endTransmission( );
}

// update degrees
void MPU6050::get_gyro( )
{
  // request gyro data
  Wire.beginTransmission( MPU_ADDR );
  Wire.write( MPU_GYRO_XOUT_H );
  Wire.endTransmission( );
  // receieve gyro data
  uint8_t rqst_sz = MPU_GYRO_XOUT_H - MPU_GYRO_ZOUT_L + 1;
  Wire.requestFrom( MPU_ADDR, rqst_sz );
  while( Wire.available( ) < rqst_sz ) { };
  // record gyro data
  gyro_x = Wire.read( )<<8 | Wire.read( );
  gyro_y = Wire.read( )<<8 | Wire.read( );
  gyro_z = Wire.read( )<<8 | Wire.read( );
  // process gyro data
  rot_x = gyro_x / 131.0;
  rot_y = gyro_y / 131.0;
  rot_z = gyro_z / 131.0;
}

// update acceleration
void MPU6050::get_accel( )
{
  // request accel data
  Wire.beginTransmission( MPU_ADDR );
  Wire.write( MPU_ACCEL_XOUT_H );
  Wire.endTransmission( );
  // recieve accel data
  uint8_t rqst_sz = MPU_ACCEL_ZOUT_L - MPU_ACCEL_XOUT_H + 1;
  Wire.requestFrom( MPU_ADDR, rqst_sz );
  while( Wire.available( ) < rqst_sz ) { };
  // record accel data
  accel_x = Wire.read( )<<8 | Wire.read( );
  accel_y = Wire.read( )<<8 | Wire.read( );
  accel_z = Wire.read( )<<8 | Wire.read( );
  // process accel data
  gforce_x = accel_x / 16384.0f;
  gforce_y = accel_y / 16384.0f;
  gforce_z = accel_z / 16384.0f;
}

// transmit
void MPU6050::transmit_data( )
{
  Serial.print( "gyro (deg): %f, %f, %f\n", rot_x, rot_y, rot_z );
  Serial.print( "gforce: %f, %f, %f\n", gforce_x, gforce_y, gforce_z );
}
