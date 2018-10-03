#include <FastLED.h>

#define DATA_PIN 5
#define NUM_LEDS 256

CRGB leds[NUM_LEDS];
CHSV syBlueHsv( 225, 50, 25);
CRGB syBlueRgb( 32, 40, 64);

CRGB rgbConverted1;
CRGB rgbConverted2;




void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  hsv2rgb_spectrum( syBlueHsv, rgbConverted1);
  hsv2rgb_rainbow( syBlueHsv, rgbConverted2);
}

void testSingleColors() {
  leds[0] = syBlueHsv;
  leds[1] = syBlueRgb;
  leds[2] = rgbConverted1;
  leds[3] = rgbConverted2;
  leds[4] = 0x202840;
}

void loop() {
  // testSingleColors();
  showHSVLogo();
  // showRGBLogo();
}
