#include <SPI.h>              // for control of light strip
#include <Adafruit_DotStar.h> // DotStar LED light strip specific
#include <Debug.h>            // for verbose debugging 

#define NUMPIXELS 60 // Number of LEDs in strip (depends on what it's cut down to)
#define TOTAL_NUMPIXELS 60

//Control the LEDs from any two pins (bit-banged SPI):
#define DATAPIN    2 //green wire
#define CLOCKPIN   4 //clock yellow

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

// color constants
const uint32_t off = 0x000000; 
const uint32_t red = 0xFF0000;      
const uint32_t green = 0x00FF00;
const uint32_t blue = 0x0000FF;   

bool setLightColor(uint32_t color){
  for (int i=0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, color);
  }
  strip.show(); // Update strip (light it up)
  return true;
}
