#include "BmpDrawer.h"
#include "RGBConverter.h"

#define BUFFPIXEL 128
BmpDrawer::BmpDrawer(struct CRGB *leds) {
  m_LED = leds;
}

BmpDrawer::BmpDrawer(uint16_t w, uint16_t h) {
  struct CRGB p_LED[(w * h)];
  CRGB leds[256];
  Serial.println(sizeof(leds));
  m_LED = &leds[0];
  Serial.println(sizeof(m_LED));
  
  _gridWidth = w;
  _gridHeight= h;
}

struct CRGB* BmpDrawer::getBuffer() {
  return (&m_LED[0]);
}

void BmpDrawer::test() {
  
}

void BmpDrawer::showFrame() { 
  
  Serial.println("showFrame");
  
  showFrame(_frameIndex);
  _frameIndex++;
  if(_frameIndex >= _totalFrames) {
    _frameIndex = 0;
  }
}

void BmpDrawer::showFrame(uint32_t index) {
  Serial.print("showFrame: "); Serial.print(index);
  Serial.println();
  if(index < 0) {
    index = _totalFrames;
  }
  
  if(index >= _totalFrames) {
    index = 0;
  }
  
  draw(0, index * 16);
}

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
    
    _totalFrames = _bmpHeight/16;
    
    Serial.print(F("Width: "));
    Serial.println(_bmpWidth  );
    Serial.print(F("Height: "));
    Serial.println(_bmpHeight);
    
    _rowSize = (_bmpWidth * 3 + 3) & ~3;
    Serial.print(F("Row size: "));
    Serial.println(_rowSize);

    Serial.print(F("Total frames: "));
    Serial.println(_totalFrames);
    

    int planes =  read16(_file);
    
    Serial.println(planes);
    if(planes == 1) { // # planes -- must be '1'
      int bitPP = read16(_file); // bits per pixel
      Serial.print(F("BPP: "));
      Serial.println(bitPP);
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

  uint8_t  sdbuffer[3]; // pixel buffer (R+G+B per pixel)
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


  buffidx = sizeof(sdbuffer); // Force buffer reload
  Serial.println(buffidx);
  _file.seekSet(_bmpImageoffset);
  
  for (row=0; row < h; row++) {
    pos = (_bmpImageoffset + (offsetX * -3) + (_bmpHeight - 1 - (row + offsetY)) * _rowSize);

    // bottom to top, so wee need to seek backwards through the bmp file
    _file.seekSet(pos);
    
    for (col=0; col<w; col++) {
      _file.read(sdbuffer, sizeof(sdbuffer));

      // push to LED buffer
      b = sdbuffer[0];
      g = sdbuffer[1];
      r = sdbuffer[2];

//      r = 100;
//      g = 0;
//      b = 0;
      double h, s, v;
      
      RGBConverter::rgbToHsv(r/255.0, g/255.0, b/255.0, &h, &s, &v);
      
      byte index = getIndex(col, row);
      
      m_LED[index] = CHSV( h*255, s* 255, v * 255);
//      m_LED[index] = CRGB( r, g, b);
//      Serial.print(F("draw2"));
//      m_LED[index] = CRGB::Red;
      
//      Serial.println(index);
//      Serial.print(r);Serial.print('-');Serial.print(g);Serial.print('-');Serial.print(b);

//      m_LED[cy * row + cx] = CRGB(255,0,0);
//      Serial.println(cy * row + cx);
////      m_LED[1] = CRGB(0,255,0);
//      
//      Serial.print(cx);Serial.print("x");Serial.print(cy);
//      Serial.print(": ");
//
//      Serial.print(r);Serial.print('-');Serial.print(g);Serial.print('-');Serial.print(b);
//      Serial.println();
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

byte BmpDrawer::getIndex(byte col, byte row)
{
  byte index;
  int columns = 16;
  
  int flip = row % 2;
  
  if(flip == 1) {
     index  = row * columns + (15 - col);
  }else{
     index = row * columns + col;
  }
  
  return index;
}
