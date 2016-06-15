#include <Adafruit_NeoPixel.h>

#define PIN_LED_OUT 6     // dig. PWM out pin for LED control
#define N_LED 60          // number of LEDs per strand. (0-59)
#define PIN_FREQ 2        // Frequency read pin. Pin 2 isn't PWM and can use ArrachInterrupt()

// Side note: 800kHz is a 1.25 us period, so delay times and sampling won't ever really approach that limit (here)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800); //initialize. RGB, 800kHz data rate

static void fadeLoop();

void setup() {
  strip.begin();  //Thrilling.
  strip.show();   // all show default (0,0,0) values. Helps with turning on the board
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
  // I can probably ditch this in general by saying (uint8_t)intensity >> rgb * 8 (for a byte) which will take care of positioning.
  if (rgb == 0xFF) {
    uint32_t color = strip.Color(255,0,0);
  } else if (rgb == 0x00FF) {
    uint32_t color = strip.Color(0,255,0);
  } else if (rgb == 0x0000FF) {
    uint32_t color = strip.Color(0,0,255);
  } else {
    return; // break out of fade loop for invalid color
  }
  
  for(uint16_t i = 0; i < strip.numPixels()+4; i++) {
    strip.setPixelColor(i, color);
    // I still need a way to dim the previous LEDs here. But the color code is a 32-bit compressed code.
    // and nobody on the wide open internet seems to be able to explain it.
    // (I already looked in the lib, it returns: " ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b; "
    // Update: it's just writing the rgb color codes through bit shifting into a 32-bit variable.
    // With a little bit (hah) of evil type management, it could be slimmed down to 24 bits (wrgb vs rgb).
    // I doubt I'll have memeory issues with such things. Still, 3 bytes vs 4 bytes multiplied by at least 60 is quite a bit of s(h)avings.
    
    strip.show(); // fin
    delay(40);    // 40ms delay
  }
}

// I think I'm going to have to halve the values of the rgb code.
// Originally I was thinking of doing a simple mask+read -> subtract fixed number (they all start at 255, would have been easy)
//  such as (0xFF & rgb) - 0x32. Then write to unint32_t.
// I could use the lib more and call reducePrevBright to setPixelColor.
// ^ then I could just use the same 'return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;' code directly into setPixelColor().
// Yay functional programming!
uint32_t halvePreviousBrightness(uint16_t &n, uint8_t &colorMask) { // num is 16 bits instead of 8 because there is a 144 LED/m version that the lib supports
  // n = pixel number
  // mask r (0x0000FF) -> halve
  // mask g (0x00FF) -> halve
  // mask b (0xFF) -> halve
  // write final value through shift. Screw the upper byte, who needs white pixels. This ain't no Sharp TV (which uses rgby anyhow).

  // However. This is the first iteration of this and I already know what value is passed.
    // I could make this more functional still and stack another frame to handle. Or just a really long return line.
  uint8_t newColor = (strip.getPixelColor(n) & colorMask) / 2;
  
}

// I'm going to try a more general catch-all version (to be used in tach mayhaps) down here because 
// I just realized that to determine the shift in the bytes from only passing an 8-bit color mask,
// I'd have to pass rgb from loop() and multiply the shift (i.e. >> rgb * 8).
// and I'd need another pointer for the function.
// Now all I need it the pixel number! yay!
uint32_t tst_halvePreviousBrightness(uint16_t &n) {
  // another note on memory, this can be done in ~1/3rd the memory by using a uint8_t* rgb pointer and storing the shifts in *this to be returned.
  // but alas, whatever.
  uint8_t r_mask = 0x0000FF;
  uint8_t g_mask = 0x00FF;
  uint8_t b_mask = 0xFF;
  
  // reuse vars because the mask is only needed once
  r_mask = (strip.GetPixelColor(n) & r_mask) / 2; //get the pixel 32-bit color. Read w/ mask. Halve brightness. Store in self.
  g_mask = (strip.GetPixelColor(n) & r_mask) / 2;
  b_mask = (strip.GetPixelColor(n) & r_mask) / 2; // I could technically do this in the return line.
                                                  // Also, I could probably devise an algorithm to just use binary operators to dim it with a custom mask.
                                                  // might be worth looking into. Maybe.

  return ((uint32_t)r_mask << 16) | ((uint32_t)g_mask <<  8) | b_mask;
}

