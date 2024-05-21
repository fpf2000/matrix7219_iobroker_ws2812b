#include <FastLED.h>

int bat = 0 ;

// FastLED PIN
#define DATA_PIN    D6
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    8
#define BRIGHTNESS  10
// Define the array of leds
CRGB leds[NUM_LEDS];

int r = 5;
int g = 5;
int b = 5;

void ledanz(){

  leds[0].setRGB( 0, 255, 0);
  leds[1].setRGB( 0, 255, 0);
  leds[2].setRGB( 0, 0, 255);
  leds[3].setRGB( 0, 0, 255);
  leds[4].setRGB( 255, 255, 0);
  leds[5].setRGB( 255, 255, 0);
  leds[6].setRGB( 255, 0, 0);
  leds[7].setRGB( 255, 0, 0);

  
  FastLED.show();
  
}

void ledgreen(){
   FastLED.clear();   
  
   for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
   } 
  delay(2000);

 Serial.print("GREEN");
}
void ledgreenx(){
    FastLED.clear(); 
   for(int i = 0; i < 6; i++) {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
   } 
  delay(2000);

 Serial.print("GREEN");
}
void ledyellow(){
    FastLED.clear();
   for(int i = 0; i < 5; i++) {
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Green;
    FastLED.show();
   } 
  delay(2000);

 Serial.print("YELLOW");
}

void ledyellowx(){
    FastLED.clear();
   for(int i = 0; i < 3; i++) {
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Yellow;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Yellow;
    FastLED.show();
   } 
  delay(2000);

 Serial.print("YELLOWx");
}

void ledred(){
    FastLED.clear();
    for(int i = 0; i < 1; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(500);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Red;
    FastLED.show();
   } 
  delay(2000);

 Serial.print("RED");
}
