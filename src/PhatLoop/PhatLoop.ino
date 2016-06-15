#include <Adafruit_NeoPixel.h>

#define PIN_LED_OUT 6     // dig. PWM out pin for LED control
#define N_LED 60          // number of LEDs per strand. (0-59)

// I'm out here to hit the phattest of loops.
// As in, I'm going to try to see what this lib does if I just feed it a static insctruction set to loop. Trying to modify brightness here.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LED, PIN_LED_OUT, NEO_GRB + NEO_KHZ800); //initialize. RGB, 800kHz data rate

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  PhatLoop(240);
  // this should really be a uint32_t that packed up all the color information but I'm just testing it at the momo.
}

void PhatLoop(uint8_t c) {
  for (uint8_t i = 0; i < strip.numPixels()+4; i++) {
    strip.setPixelColor(i, c, 0, 0);        // leading color
    strip.setPixelColor(i-1, (c/2), 0, 0);  // halve brightness
    strip.setPixelColor(i-2, (c/4), 0, 0);  // and again
    strip.setPixelColor(i-3, (c/8), 0, 0);  // and again
    strip.setPixelColor(i-4, 0, 0, 0);   // turn off
    //I don't have to worry about being out of bounds or checking for it because it is caught first thing in the setPixelColor() method.
    //catching it before the fn call would only put the instruction here rather than the fn frame. No difference.
    strip.show();
    delay(40);
  }
}

