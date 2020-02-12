#include <FastLED.h>
#include <SPI.h>
#include "Pattern.h"
#include "Headers.h"


FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

Pattern p1;
Rainbow r1;
addGlitter rg1;
Confetti c1;
Sinelon s1;
Juggle ja;
Bpm b1;
static uint8_t gHue = 0;
static int mPattern=0;

uint8_t BGiParams[] = {0, 0, 0, 0, NUM_LEDS}; // R, G, B, LED_start, len
uint8_t HSVParams[] = {0, 0, 0, 0, NUM_LEDS}; // H, S, V, LED_start, len
uint8_t BOWParams[] = {0, NUM_LEDS, 5, 0};  // Start, len, hue Delta
uint8_t FIRParams[] = {0, 0, 0, 0, NUM_LEDS}; // Hue, Sparking, Cooling, LED_start, len
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
bool patternEnArray[ARRAY_SIZE( gPatterns)] = {0};


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
  EVERY_N_SECONDS( 10 ) {
    p1.setVal(mPattern);
    mPattern= (p1.nextPattern()) % ARRAY_SIZE( gPatterns );
    Serial.print(mPattern);
  } // change patterns periodically

  gPatterns[mPattern]();
  //r1.rainbow(gHue);
  
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show(); 
}


static String listInfo[100];
int parseInfo[100];

void querySerial()
{
  while (Serial.available() > 0) {

    char anal = Serial.read(); 
    switch(anal)
    {
      //************************************************************
      // Serial message is: BGi (R, G, B, LED_start, LED_length)
      case'B':  {// string should start with B
      anal = Serial.read(); //Read second char and analyyze     
      if (anal == 'G'){ 
        if (Serial.read() == 'i'){
          // parse RGB
          BGiParams[0] = Serial.parseInt();     // then an ASCII number for red
          BGiParams[1] = Serial.parseInt();   // then an ASCII number for green
          BGiParams[2] = Serial.parseInt();    // then an ASCII number for blue
          BGiParams[3] = Serial.parseInt();   // then an ASCII number for start
          BGiParams[4] = Serial.parseInt();    // then an ASCII number for length
          BGi();
        }
      }

      if(anal == 'O') { 
        if (Serial.read() == 'W'){ 
        //patternEnArray[1] = true;  // Rainbow
          //if(Serial.parseInt()=='1'){
            static Rainbow *ra1= new Rainbow();
            ra1->callPattern(gHue);
          //}
          
        }
      }
      
      break;      
      }
              
      case'C':  {// string should start with C
      anal = Serial.read(); //Read second char and analyze     
      if (anal == 'O'){ 
      if (Serial.read() == 'F'){ 
        Confetti *c1= new Confetti();
        c1->drawPattern(gHue);
        }
      }
      break;
      }
  }
  
  
  } 
}

void BGi(){
  fill_solid( &(leds[BGiParams[3]]), BGiParams[4], CRGB( BGiParams[0], BGiParams[1], BGiParams[2]) );
}

void rainbow(){
  r1.callPattern(gHue);
}

void rainbowWithGlitter(){
  rg1.callPattern();
}

void confetti(){
  c1.drawPattern(gHue);
}

void sinelon(){
  s1.callPattern(gHue);
}

void juggle(){
  ja.callPattern();
}

void bpm(){
  b1.callPattern(gHue);

}
