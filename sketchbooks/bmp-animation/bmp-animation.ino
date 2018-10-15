
#include <SPI.h>
#include "SdFat.h"
#include "BmpDrawer.h"

#define SD_CS_PIN SS
#define BUTTON_NEXT_PIN 14

SdFat sd; // set filesystem
SdFile file; // set filesystem
BmpDrawer drawer;

boolean buttonPressed = false; // control button check

void setup() {
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
  drawer.draw();
}

void loop() {

}
