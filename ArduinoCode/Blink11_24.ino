#include <FastLED.h>
#define NUM_LEDS 144
CRGBArray<NUM_LEDS> leds;
int ledPin = 10;                 // LED connected to digital pin 13

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  FastLED.addLeds<NEOPIXEL,6>(leds, NUM_LEDS);
}

void loop()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second

  static uint8_t hue;
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);
    
    // let's set an led value
    //leds[i] = CHSV(hue++,255,255);
    leds[i]=CRGB::HotPink;
    FastLED.setBrightness(2);
    //fill_solid( &(leds[i]), 1 /*number of leds*/, CRGB( 255, 68, 221) );
    // now, let's first 20 leds to the top 20 leds, 
    //leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(33);
  }
}
