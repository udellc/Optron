#ifndef MPU6050_H
#def MPU6050_H

/* ******************************************************
 * Includes
 * ******************************************************/


/* ******************************************************
 * Types
 * ******************************************************/
class MPU6050
{
  private:
  public:

  float deg_x, deg_y, deg_z;
  float accel_x, accel_y, accel_z;

  void init( );

  void get_degrees( );
  void get_acceleration( );

  void transmit_data( );
  
};

#endif
