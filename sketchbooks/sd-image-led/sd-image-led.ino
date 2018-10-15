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

  nextImage();
}

void loop() {  
//  nextImage();
  drawFrame();
  delay(2000);
}

void bmpDraw(SdFile bmpFile, uint8_t x, uint8_t y) {
  uint32_t bmpImageoffset;        // Start of image data in file
  
  bmpFile.rewind();
  
  if(read16(bmpFile) == 0x4D42) {
    Serial.println("statr drawing bmp");
    
    (void)read32(bmpFile); // Read & ignore creator bytes
    
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
  }else {
   Serial.println("not a bmp");
  }
}

void drawFrame() {
  Serial.print("--- draw image: ");
  bmpDraw(currentFile, 0, 0);
}

void nextImage() {
  bool lastResult;
  char folderName[9];


  if(sd.chdir("wifi")){
    if(sd.exists("0.bmp")) {
      Serial.println("0.bmp found");
      currentFile.open("0.bmp", O_READ);
    }else{
      sd.ls();
    }
    
  }else {
    Serial.println("cant change");
  }
  
  
//  
//
//  while (lastResult = currentFile.openNext(sd.vwd(), O_READ)) {
//    if(currentFile.isDir() && !currentFile.isHidden() ) {
//      // good let open, continue drawing it
//      currentFile.getName(folderName, sizeof(folderName));
//      Serial.println(folderName);
//      sd.chdir(folderName);
//      sd.ls();
//      Serial.println(sd.exists("0.bmp"));
////      currentFile.close();
//      break;
//    }
//    // not a directory or hidden, close, try next
//    currentFile.close();
//  }
  
//  // end of folder reached, rewind
//  if(!lastResult) {
//    sd.vwd()->rewind();
//    nextImage();
//  }
  
}

uint16_t read16(SdFile& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(SdFile& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
