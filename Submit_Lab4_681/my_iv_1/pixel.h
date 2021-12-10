

#ifndef pixel_h
#define pixel_h

class Pixel {
  unsigned char r,g,b;
 public:
  void setcolor(int red,int green,int blue) {
    r=red; g=green; b=blue;
  }
  void setcolor(int rgb) {
    r=rgb>>16; g=(rgb & 0xff00)>>8; b=rgb & 0xff;
  }
  void getcolor(int &red, int &green, int &blue) {
    red=r; green=g; blue=b;
  }
  int getcolor() {
    return (r<<16)|(g<<8)|b;
  }
};

#endif
