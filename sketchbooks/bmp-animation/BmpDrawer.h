#include <FastLED.h>

#ifndef BmpDrawer_h
#define BmpDrawer_h

 #include "Arduino.h"
 #include "SdFat.h"

class BmpDrawer{
  public:
    BmpDrawer(struct CRGB *leds);
    BmpDrawer(uint16_t w, uint16_t h);
    
    void test();
    void showFrame();
    void showFrame(uint32_t index);
    struct CRGB *getBuffer(void);
    void setFile(SdFile file);
    void draw(uint32_t offsetX, uint32_t offsetY);
  private:
    uint16_t read16(SdFile& f);
    uint32_t read32(SdFile& f);
    byte getIndex(byte col, byte row);

    SdFile _file;
    boolean _valid = false;
    uint32_t _bmpWidth;
    uint32_t _bmpHeight;
    uint32_t _gridWidth;
    uint32_t _gridHeight;
    uint32_t _rowSize;
    uint32_t _bmpImageoffset;
    uint32_t _frameIndex = 0;
    uint32_t _totalFrames = 0;
    CRGB *m_LED;
};

#endif // BmwDrawer
