//==========================================================
// MUX DEMO
// Connor Hodgert
//==========================================================
#include <Wire.h>
#include <ZX_Sensor.h>

// defines:
#define PRINT_BACK_I2C  0
#define PRINT_ZX        1
    
#define MODE_0_PIN      18
#define MODE_1_PIN      16
#define MODE_CTRL_PIN   17
#define MODE_0_MUX      2
#define MODE_1_MUX      5
#define MUX_MODE_COUNT  8
#define BLINK_LED       11

#define INTERVAL    500

#define TCAADDR         0x71
#define MODE_0_ADDR     0xAA
#define ZX_ADDR         0x10
#define I2C_GARBAGE_VAL 0xF0

#define SERIAL_BAUD_RATE  19200

// globals:
unsigned long previous_millis;
unsigned long current_millis;
ZX_Sensor zx_sensor = ZX_Sensor( ZX_ADDR );

// functions:
void tcaseselect( uint8_t );
void send_data( uint8_t, uint8_t *, int );
void delay_ms( unsigned long );

void setup()
{
  // mux
  Wire.begin();
  pinMode( MODE_0_PIN, INPUT_PULLUP );
  pinMode( MODE_1_PIN, INPUT_PULLUP );
  pinMode( MODE_CTRL_PIN, OUTPUT );
  digitalWrite( MODE_CTRL_PIN, LOW ); 
  // ir sensor
  zx_sensor.init( );
  // serial
  Serial.begin( SERIAL_BAUD_RATE );
  // led
  pinMode( BLINK_LED, OUTPUT );
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
  
  // get mode
  uint8_t mode_1 = digitalRead( MODE_1_PIN );

  // teensy data
  if( mode_1 )
  {
    // led
    digitalWrite( BLINK_LED, HIGH );
    // data
    uint8_t data[] = { I2C_GARBAGE_VAL, I2C_GARBAGE_VAL, I2C_GARBAGE_VAL };
    int data_sz = 3;
    // select input to draw from
    tcaseselect( MODE_0_MUX );
    // send data to mux
    send_data( MODE_0_ADDR, data, data_sz );
    #if ( PRINT_BACK_I2C )
      // get data back
      Wire.requestFrom( (int)MODE_0_ADDR, data_sz );
      while( Wire.available( ) < data_sz ) { };
      // print data
      for( int ddx = 0; ddx < data_sz; ddx++ )
      {
        Serial.print( "Value " );
        Serial.print( ddx, DEC );
        Serial.print( ": " );
        Serial.println( Wire.read( ), HEX );
      }
      Serial.println( );
    #endif
  }
  // ir data
  else
  {
    // led
    digitalWrite( BLINK_LED, LOW );
    // read ZX
    tcaseselect( MODE_1_MUX );
    if( zx_sensor.positionAvailable() )
    {
      uint8_t zx_x = zx_sensor.readX( );
      uint8_t zx_z = zx_sensor.readZ( );
      #if ( PRINT_ZX )
        Serial.print( "ZX X: " );
        Serial.println( zx_x, DEC );
        Serial.print( "ZX Z: " );
        Serial.println( zx_z, DEC );
        Serial.println( );
      #endif
    }
  }
}

// select certain mux input
void tcaseselect( uint8_t port_num )
{
  if( port_num < MUX_MODE_COUNT ) 
  {
    Wire.beginTransmission( TCAADDR );
    Wire.write( 1<<port_num );
    Wire.endTransmission( );
  }
}

// send some data
void send_data( uint8_t addr, uint8_t *data, int data_sz )
{
  Wire.beginTransmission( addr );
  for( int ddx = 0; ddx < data_sz; ddx++ )
  {
    Wire.write( data[ddx] );
  }
  Wire.endTransmission( );
}

void delay_ms( unsigned long amnt )
{
  unsigned long goal = millis( ) + amnt;
  while( millis( ) < goal ) { }
}
