#include "BmpDrawer.h"

#define BUFFPIXEL 1

void BmpDrawer::setFile(SdFile file) {
  Serial.print("sd file open");
  _file = file;
  
}

void BmpDrawer::draw() {
  _file.rewind();

  const uint8_t  gridWidth = 16;
  const uint8_t  gridHeight = 16;
  int offsetY = 16;
  int offsetX = 0;
  
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  
  int  bmpWidth, bmpHeight;   // W+H in pixels
  uint32_t bmpImageoffset;
  uint32_t  rowSize;               // Not always = bmpWidth; may have padding
  boolean validBmp = false;
  uint16_t bmpDepth;

  int  w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;

  int cx = 0;
  int cy = 0;
 
  // Parse BMP header
  if(read16(_file) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(_file));
    (void)read32(_file); // Read & ignore creator bytes
    Serial.println();
    bmpImageoffset = read32(_file); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(_file));
    bmpWidth  = read32(_file);
    bmpHeight = read32(_file);

    int planes =  read16(_file);
    Serial.println(planes);
    if(planes == 1) { // # planes -- must be '1'
      bmpDepth = read16(_file); // bits per pixel
      validBmp = true; // Supported BMP format -- proceed!
      Serial.print(F("Image size: "));
      Serial.print(bmpWidth);
      Serial.print('x');
      Serial.println(bmpHeight);

      rowSize = (bmpWidth * 3 + 3) & ~3;
      Serial.print(F("Row size: "));
      Serial.println(rowSize);

      // Crop area to be loaded
      w = bmpWidth;
      h = bmpHeight;
      if((w-1) >= gridWidth)  w = gridWidth;
      if((h-1) >= gridHeight) h = gridHeight;
      
      
      for (row=0; row<h; row++) { 
        // Bitmap is stored bottom-to-top order (normal BMP)
        pos = (bmpImageoffset + (offsetX * -3) + (bmpHeight - 1 - (row + offsetY)) * rowSize);
        
        if(_file.curPosition() != pos) { // Need seek?
          _file.seekSet(pos);
          buffidx = sizeof(sdbuffer); // Force buffer reload
        }

        for (col=0; col<w; col++) { // For each pixel...
           // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              _file.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }
            
            // push to LED buffer 
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            cx = row + offsetX;
            cy = col + offsetY;
            Serial.print(cx);Serial.print("x");Serial.print(cy);
            Serial.print(": ");
            
            Serial.print(r);Serial.print('-');Serial.print(g);Serial.print('-');Serial.print(b);
            Serial.println();
            
        }
      }
    }else {
      Serial.println(F("Found Planes != 1"));
    }
  }else {
    Serial.println("File is not a Bitmap");
  }
}

uint16_t BmpDrawer::read16(SdFile& f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t BmpDrawer::read32(SdFile& f) {
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
