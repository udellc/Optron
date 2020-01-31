#include <FastLED.h>
#include <SPI.h>
#include "Pattern.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

Pattern p1;
Rainbow r1;
Confetti c1;
Sinelon s1;
juggle ja;
static uint8_t gHue = 0;
static uint8_t mPattern=0;

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(9600);
  //FastLED.begin(); // Initialize pins for output
  FastLED.show();  // Turn all LEDs off ASAP
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,COLOR_ORDER>(leds, NUM_LEDS);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  querySerial();
  FastLED.delay(1000/FRAMES_PER_SECOND); //1000/120=8miliseconds

  // do some periodic updates
  EVERY_N_MILLISECONDS( 100 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { p1.nextPattern(mPattern); } // change patterns periodically

  //r1.rainbow(gHue);
  c1.drawPattern(gHue);
  s1.callPattern(gHue);
  //ja.callPattern();
  //Serial.println(gHue);
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show(); 
}

void querySerial()
{
  // listen for serial:
  // set color arrays
  while (Serial.available() > 0) {
    int num = Serial.parseInt();    //parse first int as on/off for multiple animation
    if(num==0){
      break;
      r1.rainbow(gHue);
    }
    else{
    int anal = Serial.parseInt();   //call patterns// 2 for rainbow 3 for 
    int anal2= Serial.parseInt();
    //array <Under Construction>
    }
  } 
}
