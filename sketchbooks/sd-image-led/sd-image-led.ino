#include <SPI.h>
#include "SdFat.h"

#define SD_CS_PIN SS

SdFat sd;
SdFile currentFile;

int folderIndex = 0;

void setup() {
  Serial.begin(9600);

  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  delay(1000);
  
  Serial.print("Initializing SD card...");

  if (!sd.begin(SD_CS_PIN)) {
    sd.initErrorHalt();
  }
  
  Serial.println("initialization done.");
  sd.vwd()->rewind();
//  sd.ls();
//  sd.vwd()->rewind();

  
}

void loop() {  
  nextImage();
  drawFrame();
  delay(200);
}


void drawFrame() {
  
  //  bmpDraw("0.bmp", 0, 0);
  Serial.print("--- draw image: ");
  currentFile.printName(&Serial);
  
  Serial.println();


  currentFile.close();
}

void nextImage() {
  bool lastResult;
  while (lastResult = currentFile.openNext(sd.vwd(), O_READ)) {
    if(currentFile.isDir() && !currentFile.isHidden() ) {
      // good let open, continue drawing it
      break;
    }
    // not a directory or hidden, close, try next
    currentFile.close();
  }
  
  // end of folder reached, rewind
  if(!lastResult) {
    sd.vwd()->rewind();
    nextImage();
  }
  
}
