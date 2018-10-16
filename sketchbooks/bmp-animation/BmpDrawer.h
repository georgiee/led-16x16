#ifndef BmpDrawer_h
#define BmpDrawer_h

 #include "Arduino.h"
 #include "SdFat.h"

class BmpDrawer{
  public:
    void setFile(SdFile file);
    void draw(uint32_t offsetX, uint32_t offsetY);
  private:
    uint16_t read16(SdFile& f);
    uint32_t read32(SdFile& f);
    byte getIndex(byte x, byte y);

    SdFile _file;
    boolean _valid = false;
    uint32_t _bmpWidth;
    uint32_t _bmpHeight;
    uint32_t _rowSize;
    uint32_t _bmpImageoffset;
};

#endif // BmwDrawer
