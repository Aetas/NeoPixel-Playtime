#include <Adafruit_NeoPixel.h>

#define PIN 6     // dig. PWM out pin
#define N_LED 60  // number of LEDs per strand. (0-59)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800); //initialize

void setup() {
  strip.begin();  //Thrilling.
}

void loop() {
  // put your main code here, to run repeatedly:

}
