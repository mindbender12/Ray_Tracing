
#ifndef scan_h
#define scan_h

#include <fstream>
#include <iostream>
using namespace std;
#include "frame.h"
#include "jpeg.h"
#include "datastruct.h"

extern int restartinterval;
extern int eoifound;

struct MCUdata {
  int *data;
  int k;
  void setsize(int x) {
    data=new int[x];
    k=0;
  }
  void reset() { k=0; }
  void add(int x) { data[k++]=x; }
  MCUdata() { k=0; data=0; }
  MCUdata(MCUdata &o) {
    k=o.k;
    data=new int[k];
    for (int i=0;i<k;i++) data[i]=o.data[i];
  }
  void operator = (MCUdata &o) {
    k=o.k;
    data=new int[k];
    for (int i=0;i<k;i++) data[i]=o.data[i];
  }
  ~MCUdata () {
    if (data) delete [] data;
    k=0;
  }
  int &operator [] (int x) { return data[x]; }
};

int readscan(Frame &frame);
LList <MCUdata> * calscan(Frame &frame);
void writescan(LList <MCUdata> *mculist, Frame &frame);

#endif
