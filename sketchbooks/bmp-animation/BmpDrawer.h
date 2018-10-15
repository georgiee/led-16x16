#ifndef BmpDrawer_h
#define BmpDrawer_h

 #include "Arduino.h"
 #include "SdFat.h"

class BmpDrawer{
  public:
    void setFile(SdFile file);
    void draw();
  private:
    uint16_t read16(SdFile& f);
    uint32_t read32(SdFile& f);
    byte getIndex(byte x, byte y);
    SdFile _file;
};

#endif // BmwDrawer
