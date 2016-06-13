#include <Adafruit_NeoPixel.h>

#define PIN_LED_OUT 6     // dig. PWM out pin for LED control
#define N_LED 60          // number of LEDs per strand. (0-59)
#define PIN_FREQ 2        // Frequency read pin. Pin 2 isn't PWM and can use ArrachInterrupt()

// Side note: 800kHz is a 1.25 us period, so delay times and sampling won't ever really approach that limit
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800); //initialize. RGB, 800kHz data rate

static void fadeLoop();

void setup() {
  strip.begin();  //Thrilling.
  strip.show();
}

// *.Color(R, G, B) takes numerical RGB values (0-255, brigness) and returns a 32-bit color code
void loop() {
  //fadeLoop(strip.Color(255, 0, 0)); // red
  //fadeLoop(strip.Color(0, 255, 0)); // green
  //fadeLoop(strip.Color(0, 0, 255)); // blue
  fadeLoop(1);
  fadeLoop(2);
  fadeLoop(3);
}

// strip.numPixels()+4 because it cannot write to nonexistant LEDs and I need the loop to 
// complete with turning off the i-4 pixel. (and that one needs to be num 59)
// the lib actually takes care of the n > numLED conditional in setPixelColor. Yass.

// strip.setBrightness(n) should be marvelous, actually. ( NEVERMIND, this is supposed to be a setup function -_- )
// I'm only keeping 4 LEDs on at a time so I can power it from USB on my computer. 

// Even if I use this as a tachometer, it'll either be from a 9V whatever battery or the car battery.
// and considering the strip can draw ~3A at full tilt, I imagine a poor little 9V battery would fare poorly.
// I don't fancy killing my more expensive car battery either. I don't know it's operational amperage either, though it's written on top
// The alternator also supplies more or less power depending on revs...
// Might be nice, actually since the power requirements go up as the revs increase.

// These comments should really go in a *.ideas text file.
static void fadeLoop(uint8_t rgb) {
  // ugly and unfortunate, but the library doesn't have a great brightness adjusting function that can be called repeatedly
  // update note: I should be able to just handle the color library with bit access to the uint32_t color. (uint32_t)(color >> 16), etc.
  if (rgb == 1) {
    uint32_t color = strip.Color(255,0,0);
  } else if (rgb == 2) {
    uint32_t color = strip.Color(0,255,0);
  } else if (rgb == 3) {
    uint32_t color = strip.Color(0,0,255);
  } else {
    return;
  }
  
  for(uint16_t i = 0; i < strip.numPixels()+4; i++) {
    strip.setPixelColor(i, color);
    // I still need a way to dim the previous LEDs here. But the color code is a 32-bit compressed code.
    // and nobody on the wide open internet seems to be able to explain it.
    // (I already looked in the lib, it returns: " ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b; "
    // I suppose I need to do some reading. Looks like they set the color in 8-bit increments in the 32-bit var
    // But the top 8 bits are for strips with white LED support. So are they unreliable in this?
    
    strip.show(); // fin
    delay(40);    // 40ms delay 
  }
}

