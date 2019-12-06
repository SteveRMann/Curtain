#define SKETCH "Curtain8cOTA.ino"

/*
   Curtain8.ino works, but no two strings can drip at the same time, and they drip sequentially.
   8a is an attempt t0 randomize the calls to drip.
   8b Makes the randomize simpler.  Rather than depend on interval, just randomize the calls to drip_n.
   8c inverts the "flow" of the drip from the end to the start of the string.
*/

//Possible flicker fixes:
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include "D:\River Documents\Arduino\libraries\Kaywinnet.h"
#include "ESP8266WiFi.h"
#include <ArduinoOTA.h>

#define DEBUG true  //set to true for debug output, false for no debug ouput
#define Serial if(DEBUG)Serial


#define NUM_LEDS 25     // Per string
#define DATA_PIN1 D1
#define DATA_PIN2 D2
#define DATA_PIN3 D3
#define DATA_PIN4 D4
#define DATA_PIN5 D5
#define DATA_PIN6 D6
#define DATA_PIN7 D7
#define DATA_PIN8 D8
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER RGB


CRGB leds_1[NUM_LEDS];        // This is an array of leds.  One item for each led in your strip.
CRGB leds_1_Saved;            // Save the LED color here (for the drip function)
CRGB leds_2[NUM_LEDS];
CRGB leds_2_Saved;
CRGB leds_3[NUM_LEDS];
CRGB leds_3_Saved;
CRGB leds_4[NUM_LEDS];
CRGB leds_4_Saved;
CRGB leds_5[NUM_LEDS];
CRGB leds_5_Saved;
CRGB leds_6[NUM_LEDS];
CRGB leds_6_Saved;
CRGB leds_7[NUM_LEDS];
CRGB leds_7_Saved;
CRGB leds_8[NUM_LEDS];
CRGB leds_8_Saved;


// Variable holding the timer value so far. One for each string of LEDs
unsigned long leds_1_timer;
unsigned long leds_2_timer;
unsigned long leds_3_timer;
unsigned long leds_4_timer;
unsigned long leds_5_timer;
unsigned long leds_6_timer;
unsigned long leds_7_timer;
unsigned long leds_8_timer;

// Time periods of drips in milliseconds
unsigned long leds_1_Interval;
unsigned long leds_2_Interval;
unsigned long leds_3_Interval;
unsigned long leds_4_Interval;
unsigned long leds_5_Interval;
unsigned long leds_6_Interval;
unsigned long leds_7_Interval;
unsigned long leds_8_Interval;


unsigned long dripSpeedFast = 2U;
unsigned long dripSpeedSlow = 75U;

// Delay between drips (ms)
int intervalMin = 100;
int intervalMax = 500;

#define CurtainColor CRGB(0x001bff)  //Blue (r=00, g=1b, b=ff)

#define MGPu CRGB(0x8010ff) // Mardi Gras Purple
#define MGGr CRGB(0x2cb004) // Green
#define MGGo CRGB(0xff4a00) // Gold





// =========================== setup() ===========================
void setup() {
  Serial.begin(115200);
  Serial.println();
#ifdef SKETCH
  Serial.println(F(SKETCH));
#endif
  Serial.println(F("---------------"));

  setup_wifi();
  start_OTA();


  // The initial interval between drips in milliseconds
  leds_1_Interval = random(intervalMin, intervalMax);
  leds_2_Interval = random(intervalMin, intervalMax);
  leds_3_Interval = random(intervalMin, intervalMax);
  leds_4_Interval = random(intervalMin, intervalMax);
  leds_5_Interval = random(intervalMin, intervalMax);
  leds_6_Interval = random(intervalMin, intervalMax);
  leds_7_Interval = random(intervalMin, intervalMax);
  leds_8_Interval = random(intervalMin, intervalMax);

  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(1000);

  // This tells the library that there's a strand of LED_TYPE on pin DATA_PIN1 to DATA_PIN3,
  // and those leds will use the led array leds_1 to leds_3, and there are NUM_LEDS of them.
  FastLED.addLeds<LED_TYPE, DATA_PIN1, COLOR_ORDER>(leds_1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds_2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN3, COLOR_ORDER>(leds_3, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN4, COLOR_ORDER>(leds_4, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN5, COLOR_ORDER>(leds_5, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN6, COLOR_ORDER>(leds_6, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN7, COLOR_ORDER>(leds_7, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN8, COLOR_ORDER>(leds_8, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);


  // Start with all LEDs off.
  oneColor(CRGB::Black);
  delay(500);

  // Curtain color- the LEDs normal color
  //oneColor(CRGB::Blue);
  oneColor(CurtainColor);

  leds_1_timer = millis ();
  leds_2_timer = millis ();
  leds_3_timer = millis ();
  leds_4_timer = millis ();
  leds_5_timer = millis ();
  leds_6_timer = millis ();
  leds_7_timer = millis ();
  leds_8_timer = millis ();
}


// =================== loop() ===================
void loop() {
#ifdef __ARDUINO_OTA_H
  ArduinoOTA.handle();
#endif
  int lastDrip = 0;
  //  int i = rrandom (1, 8);           // Drip random strings, no duplicates
  int i = rrandom (1, 4);           // Drip random strings, no duplicates (Just strings 1-4)
  switch (i) {
    case 1:
      drip_1();
      break;
    case 2:
      drip_2();
      break;
    case 3:
      drip_3();
      break;
    case 4:
      drip_4();
      break;
    case 5:
      drip_5();
      break;
    case 6:
      drip_6();
      break;
    case 7:
      drip_7();
      break;
    case 8:
      drip_8();
      break;
  }

  delay(random(intervalMin, intervalMax));

}



// =========================== oneColor() ===========================
void oneColor(CRGB myColor) {
  // Light the leds one at a time. Leave them on.
  for (int ledNum = 0; ledNum < NUM_LEDS; ledNum = ledNum + 1) {
    leds_1[ledNum] = myColor;
    leds_2[ledNum] = myColor;
    leds_3[ledNum] = myColor;
    leds_4[ledNum] = myColor;
    leds_5[ledNum] = myColor;
    leds_6[ledNum] = myColor;
    leds_7[ledNum] = myColor;
    leds_8[ledNum] = myColor;
    FastLED.show();
    delay(10);        // Not needed, but you can see the leds light up sequentially.
  }
}


// =========================== drip_1() ===========================
// Drip a white LED along the string.
void drip_1() {
  // For each LED
  // save the current color, set the LED to white, delay, then restore the color.
  Serial.println(F("drip_1..."));
  static int i;

  //for (i = 0; i <= NUM_LEDS; i++) {
  for (i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    Serial.println(i);
    leds_1_Saved = leds_1[i];
    leds_1[i] = CRGB::White;
    FastLED.show();

    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_1[i] = leds_1_Saved;
    FastLED.show();

  }
  leds_1_timer = millis();       // remember when we dripped
  leds_1_Interval = random(intervalMin, intervalMax);
}


// =========================== drip_2() ===========================
// Drip a white LED along the string.
void drip_2() {
  // For each LED
  // save the current color, set the LED to white, delay, then restore the color.

  //for (int i = 0; i <= NUM_LEDS; i++) {
  for (int i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_2_Saved = leds_2[i];
    leds_2[i] = CRGB::White;
    FastLED.show();
    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_2[i] = leds_2_Saved;
    FastLED.show();
  }
  leds_2_timer = millis();                          // remember when we dripped
  leds_2_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_2_Interval);
}



// =========================== drip_3() ===========================
// Drip a white LED along the string.
void drip_3() {
  //for (int i = 0; i <= NUM_LEDS; i++) {
  for (int i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_3_Saved = leds_3[i];
    leds_3[i] = CRGB::White;
    FastLED.show();
    delay(random(dripSpeedFast, dripSpeedSlow));    // Speed of the drip.  Lower is faster
    leds_3[i] = leds_3_Saved;
    FastLED.show();
  }
  leds_3_timer = millis();                      // remember when we dripped
  leds_3_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_3_Interval);
}



// =========================== drip_4() ===========================
void drip_4() {
  //for (int i = 0; i <= NUM_LEDS; i++) {
  for (int i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_4_Saved = leds_4[i];
    leds_4[i] = CRGB::White;
    FastLED.show();
    delay(random(dripSpeedFast, dripSpeedSlow));  // Speed of the drip.  Lower is faster
    leds_4[i] = leds_4_Saved;
    FastLED.show();
  }
  leds_4_timer = millis();                      // remember when we dripped
  leds_4_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_4_Interval);
}


// =========================== drip_5() ===========================
void drip_5() {
  static int i;

  //for (i = 0; i <= NUM_LEDS; i++) {
  for (i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_5_Saved = leds_5[i];
    leds_5[i] = CRGB::White;
    FastLED.show();

    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_5[i] = leds_5_Saved;
    FastLED.show();
  }
  leds_5_timer = millis();       // remember when we dripped
  leds_5_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_5_Interval);
}


// =========================== drip_6() ===========================
void drip_6() {
  static int i;

  //for (i = 0; i <= NUM_LEDS; i++) {
  for (i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_6_Saved = leds_6[i];
    leds_6[i] = CRGB::White;
    FastLED.show();

    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_6[i] = leds_6_Saved;
    FastLED.show();

  }
  leds_6_timer = millis();       // remember when we dripped
  leds_6_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_6_Interval);
}


// =========================== drip_7() ===========================
void drip_7() {
  static int i;

  //for (i = 0; i <= NUM_LEDS; i++) {
  for (i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_7_Saved = leds_7[i];
    leds_7[i] = CRGB::White;
    FastLED.show();

    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_7[i] = leds_7_Saved;
    FastLED.show();

  }
  leds_7_timer = millis();       // remember when we dripped
  leds_7_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_7_Interval);
}


// =========================== drip_8() ===========================
void drip_8() {
  static int i;

  //for (i = 0; i <= NUM_LEDS; i++) {
  for (i = NUM_LEDS; i >= 0; i--) {           // Reverse the direction of the drip
    leds_8_Saved = leds_8[i];
    leds_8[i] = CRGB::White;
    FastLED.show();

    delay(random(dripSpeedFast, dripSpeedSlow));      // Speed of the drip.  Lower is faster
    leds_8[i] = leds_8_Saved;
    FastLED.show();

  }
  leds_8_timer = millis();       // remember when we dripped
  leds_8_Interval = random(intervalMin, intervalMax);
  //Serial.print(F("  Interval= "));
  //Serial.println(leds_8_Interval);
}


// =========================== rrandom() ===========================
// Generate a random number with no duplicates.
int rrandom(int low, int high) {
  // Generate a random number between 'low' and 'high'
  static int previous_i = 0;
  static int i = random(low - 1, high + 1);
  while (i == previous_i || i == low - 1 || i == high + 1) {
    i = random(low - 1, high + 1);
  }
  previous_i = i;
  return i;
}
