/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <FastLED.h>
#include "RGBConverter.h"

#define BUFFPIXEL 1
#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"
SdFat SD;

#define SD_CS_PIN SS
File myFile;

#define NUM_LEDS 256
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

#define BUTTON_NEXT_PIN 14

boolean 
  buttonPressed = false; // control button check
  
void setup() {
  pinMode(BUTTON_NEXT_PIN, INPUT); 
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  // re-open the file for reading:
  myFile = SD.open("testimage2.bmp");
  if (myFile) {
    Serial.println("test.txt:");

//    bmpDraw("testimage2.bmp", 0, 0, leds);
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test-framer-01.bmp");
  }
}

void loop() {

  if(digitalRead(BUTTON_NEXT_PIN) == LOW && buttonPressed == false) {
    Serial.println("button pressed");
    myFile = SD.open("testimage2.bmp");
    bmpDraw("testimage2.bmp", 0, 0, leds);
    
    myFile.close();
    buttonPressed = true;
  }else if(digitalRead(BUTTON_NEXT_PIN) == HIGH && buttonPressed == true) {
    
    myFile = SD.open("heart.bmp");
    bmpDraw("heart.bmps", 0, 0, leds);
    
    myFile.close();
    buttonPressed = false;
  }
//  for(int i = 0; i < NUM_LEDS;i++) {
//    leds[i] = CRGB(255,255,0);
//  }
//  FastLED.show();
//  delay(100);
//  ledData[getIndex(col, row)] = CRGB(255,255,0);
  // nothing happens after setup
}


void bmpDraw(char *filename, uint8_t x, uint8_t y, struct CRGB *ledData) {

  int  bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t  rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int  w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;
  const uint8_t  gridWidth = 16;
  const uint8_t  gridHeight = 16;
  
  
  if((x >= gridWidth) || (y >= gridHeight)) {
    Serial.print(F("Abort."));
    return;
  }
  
  Serial.println();
  
  if (!myFile.isOpen())
  {
    Serial.print(F("Loading image '"));
    Serial.print(filename);
    Serial.println('\'');
    // Open requested file on SD card
    if (!myFile.open(filename, O_READ)) {
      Serial.println(F("File open failed"));
      return;
    }
  }
  else myFile.rewind();
  


  // Parse BMP header
  if(read16(myFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(myFile));
    (void)read32(myFile); // Read & ignore creator bytes
    bmpImageoffset = read32(myFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(myFile));
    bmpWidth  = read32(myFile);
    bmpHeight = read32(myFile);
    if(read16(myFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(myFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(myFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        

        
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;
        Serial.print(F("Row size: "));
        Serial.println(rowSize);

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }
        
        // initialize our pixel index
//        byte index = 0; // a byte is perfect for a 16x16 grid

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= gridWidth)  w = gridWidth - x;
        if((y+h-1) >= gridHeight) h = gridHeight - y;

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = (bmpImageoffset + (bmpHeight - row -1) * rowSize);
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          
//          pos = bmpImageoffset + row * rowSize;
Serial.println(pos);

          if(myFile.curPosition() != pos) { // Need seek?
            myFile.seekSet(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }
           
          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              myFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }
            // push to LED buffer 
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            Serial.print(F("Index"));
        Serial.print(getIndex(col, row));
        Serial.print('(');
        Serial.print(col);
        Serial.print('-');
        Serial.print(row);
        Serial.print(')');
        Serial.print(':');
        Serial.print(r);
        Serial.print(',');
        Serial.print(g);
        Serial.print(',');
        Serial.print(b);
        Serial.println(' ');
            double hsv;
            double h, s, v;
            
            RGBConverter::rgbToHsv(r/255, g/255, b/255, &h, &s, &v);
        
        Serial.println(h);
//            ledData[getIndex(col, row)] = CHSV( h, s * 255, v * 255);
             ledData[getIndex(col, row)] = CRGB(r,g,b);
          
            // all good
//            else strip.setPixelColor(getIndex(col+x, row), strip.Color(r, g, b));
            // paint pixel color
          } // end pixel
        } // end scanline
      } // end goodBmp
    }
  }
  
  FastLED.show();
 
  if(!goodBmp) Serial.println(F("Format unrecognized."));
}




// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File& f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

byte getIndex(byte x, byte y)
{
  byte index;
  if (y == 0)
  {
    index = 15 - x;
  }
  else if (y % 2 != 0)
  {
    index = y * 16 + x;
  }
  else
  {
    index = (y * 16 + 15) - x;
  }
  return index;
}
