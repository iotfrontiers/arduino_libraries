/**
 * 좌표 == Y + (X * Hegiht)
 */

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <GlassLedMatrix.h>
#include <Fonts/GodoM6pt8b.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define WIDTH 24
#define HEIGHT 12

GlassLedMatrix matrix(25, WIDTH, HEIGHT); //  pin, width, height

void setup() {
  Serial.begin(115200);
  Serial.println("setup!11111111111");

  setupFont();
  // matrix.clear();
  // matrix.println("power");
}


void loop() {
    delay(1000);
  for (int i = WIDTH; i > -100; i--) {
    matrix.clear();
    matrix.setCursor(i,1);
    matrix.print("p2sg global fighting!");
    matrix.show();
    delay(100);
  }

}

void setupFont() {
  // matrix.setFont(&GodoM6pt8b);  
  // matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
  // matrix.setAttribute(UTF8_ENABLE , true);
}
