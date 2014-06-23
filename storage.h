#ifndef storage_h
#define storage_h

#include "Arduino.h"


class StorageClass {
  public:
    StorageClass();
    void begin();
    void test();
  private:
};

extern StorageClass Storage;
#endif
