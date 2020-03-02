//==========================================================
// IR ACCEL DEMO
// Connor Hodgert
//==========================================================
#include "Wire.h"
#include <MPU6050.h>
#include <ZX_Sensor.h>

// defines:
#define MPU 0
#define IR 1
#define INTERVAL        100
#define SERIAL_BAUD_RATE  19200
#define MPU_ADDR 0x68
#define ZX_ADDR 0x10
#define TCAADDR 0x71
#define INTERRUPT_PIN 11
#define IR_COUNT 7

// globals:
unsigned long previous_millis;
unsigned long current_millis;
// mpu
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
// IR
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
uint8_t x_pos[IR_COUNT];
uint8_t z_pos[IR_COUNT];

// functions:
void measure_mpu6050();
void readXZ();
void tcaseselect(uint8_t);

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void setup()
{
  // serial
  Serial.begin( SERIAL_BAUD_RATE );
  
  // notify
  Serial.println( "SETUP STARTING" );
  
  Wire.begin();
  delay( 1000 );
  
  // mpu
  #if MPU
    accelgyro.initialize();
    accelgyro.setZAccelOffset(1000); // 1688 factory default for my test chip
    accelgyro.setXGyroOffset(140); // was 220, 120=-.25, 180=.56, 150=.19, 135=-.06, 140=0
    accelgyro.setYGyroOffset(-10); // 76 = 1.192, 100=1.5, 50=0.8, 20=.35, -20=-.15, -10=0
    accelgyro.setZGyroOffset(60); //-75=-1.9, -20=-1., 20=-.5, 40=-.2, 60=.0
    delay( 1000 );
  #endif
  // IR
  #if IR
    for (uint8_t t=0; t<IR_COUNT; t++) 
    {
      tcaseselect(t);
      return;
      Wire.beginTransmission(ZX_ADDR);
      byte error = Wire.endTransmission();
      if (error == 0)
      {
         zx_sensor.init();
      }
    }
  #endif
  
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

  // notify
  Serial.println( "RUN" );
  return;

  
  #if MPU
    // get mpu
    measure_mpu6050( );
    // print mpu
    Serial.print( "AX: " ); Serial.print( ax ); Serial.print( "  " );
    Serial.print( "AY: " ); Serial.print( ay ); Serial.print( "  " );
    Serial.print( "AZ: " ); Serial.print( az ); Serial.println( );
    Serial.print( "GX: " ); Serial.print( gx ); Serial.print( "  " );
    Serial.print( "GY: " ); Serial.print( gy ); Serial.print( "  " );
    Serial.print( "GZ: " ); Serial.print( gz ); Serial.println( );
  #endif

  #if IR
    // get IR:
    readXZ( );
    // print IR:
    for (uint8_t t=0; t<IR_COUNT; t++) 
    {
      Serial.print( "ZX SENSOR: < " ); Serial.print( x_pos[t] ); Serial.print( ", " ); Serial.print( z_pos[t] ); Serial.println( " >" );
    }
  #endif
}

void measure_mpu6050()
{
    accelgyro.resetFIFO();
    
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
}

void readXZ()
{
  // TCA loop to read all 8 sensor ports
  for (uint8_t t=0; t<8; t++) {
    tcaseselect(t);
    // If there is position data available, read and print it
    if ( zx_sensor.positionAvailable() ) {
      x_pos[t] = zx_sensor.readX();
      z_pos[t] = zx_sensor.readZ();
    }
    
  } // end TCA loop
}

void tcaseselect(uint8_t port_num)
{
  if (port_num < IR_COUNT) {
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << port_num);
    Wire.endTransmission();
  }
}
