#ifndef MPU6050_H
#define MPU6050_H

/* ******************************************************
 * Includes
 * ******************************************************/
#include <Wire.h>

/* ******************************************************
 * MPU6050 register mapping
 * ******************************************************/
#define MPU_ADDR 0b1101000

#define MPU_PWR_MGMT_1 0x6B
#define MPU_DEVICE_RESET 0x80
#define MPU_SLEEP 0x40
#define MPU_CYCLE 0x20
#define MPU_TEMP_DIS 0x08
#define MPU_CLKSEL2 0x04
#define MPU_CLKSEL1 0x02
#define MPU_CLKSEL0 0x01

#define MPU_GYRO_CONFIG 0x1B
#define MPU_XG_ST 0x80
#define MPU_YG_ST 0x40
#define MPU_ZG_ST 0x20
#define MPU_FS_SEL1 0x10
#define MPU_FS_SEL0 0x08

#define MPU_ACCEL_CONFIG 0x1C
#define MPU_XA_ST 0x80
#define MPU_YA_ST 0x40
#define MPU_ZA_ST 0x20
#define MPU_AFS_SEL1 0x10
#define MPU_AFS_SEL0 0x08

#define MPU_ACCEL_XOUT_H 0x3B
#define MPU_ACCEL_XOUT_L 0x3C
#define MPU_ACCEL_YOUT_H 0x3D
#define MPU_ACCEL_YOUT_L 0x3E
#define MPU_ACCEL_ZOUT_H 0x3F
#define MPU_ACCEL_ZOUT_L 0x40

#define MPU_GYRO_XOUT_H 0x43
#define MPU_GYRO_XOUT_L 0x44
#define MPU_GYRO_YOUT_H 0x45
#define MPU_GYRO_YOUT_L 0x46
#define MPU_GYRO_ZOUT_H 0x47
#define MPU_GYRO_ZOUT_L 0x48

/* ******************************************************
 * Types
 * ******************************************************/
class MPU6050
{ 
  public:

    float gyro_x, gyro_y, gyro_z;  // raw
    float rot_x, rot_y, rot_z;  // refined (deg)
    
    float accel_x, accel_y, accel_z;  // raw
    float gforce_x, gforce_y, gforce_z;  // refined
  
    void init( );
  
    void get_gyro( );
    void get_accel( );
  
    void transmit_data( );
  
};

#endif
