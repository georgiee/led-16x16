#include "BmpDrawer.h"

#define BUFFPIXEL 1

void BmpDrawer::setFile(SdFile file) {
  Serial.print("sd file open");
  _file = file;

  if(read16(_file) == 0x4D42) {
    Serial.print(F("File size: ")); Serial.println(read32(_file));
    (void)read32(_file); // Read & ignore creator bytes
    Serial.println();
    _bmpImageoffset = read32(_file); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(_bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(_file));
    _bmpWidth  = read32(_file);
    _bmpHeight = read32(_file);

    _rowSize = (_bmpWidth * 3 + 3) & ~3;
    Serial.print(F("Row size: "));
    Serial.println(_rowSize);

    int planes =  read16(_file);
    Serial.println(planes);
    if(planes == 1) { // # planes -- must be '1'
      read16(_file); // bits per pixel
      _valid = true; // Supported BMP format -- proceed!

    }
  }
}
void BmpDrawer::draw(uint32_t offsetX, uint32_t offsetY) {
  const uint8_t  gridWidth = 16;
  const uint8_t  gridHeight = 16;

  uint32_t pos = 0;
  int  w, h, row, col;
  uint8_t  r, g, b;

  int cx = 0;
  int cy = 0;

  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer

  if(_valid == false) {
    Serial.print(F("Can't draw, BMP invalid"));
    return;
  }

  Serial.print(F("BMP valid, drawing:"));
  Serial.print(F("Image size: "));
  Serial.print(_bmpWidth);
  Serial.print('x');
  Serial.println(_bmpHeight);

  _file.rewind();

  // Crop area to be loaded
  w = _bmpWidth;
  h = _bmpHeight;
  if((w-1) >= gridWidth)  w = gridWidth;
  if((h-1) >= gridHeight) h = gridHeight;


  for (row=0; row < h; row++) {
    // Bitmap is stored bottom-to-top order (normal BMP)
    pos = (_bmpImageoffset + (offsetX * -3) + (_bmpHeight - 1 - (row + offsetY)) * _rowSize);

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
