#include <FastLED.h>
#include <SPI.h>
#include "SdFat.h"
#include "BmpDrawer.h"

#define NUM_LEDS 256
#define LED_PIN 2
#define SD_CS_PIN SS
#define BUTTON_NEXT_PIN 14

CRGB leds[NUM_LEDS];
#define BRIGHTNESS 150

SdFat sd; // set filesystem
SdFile file; // set filesystem
//BmpDrawer drawer(16, 16);
BmpDrawer drawer(leds);
boolean buttonPressed = false; // control button check

struct CRGB *m_LED;

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, 256);
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode(BUTTON_NEXT_PIN, INPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!sd.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");
  openImage();
}

void openImage() {
  if (!file.open("wifi/0.bmp", O_READ)) {
    Serial.println(F("File open failed"));
    return;
  }

  drawer.setFile(file);
  drawer.showFrame(0);
  FastLED.show();
}

void loop() {
  drawer.showFrame();
  FastLED.show();
  delay(20);
}
