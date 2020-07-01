#ifndef ARDUINO_H
#include <Arduino.h>
#endif
class giudittaLedLamps{
  public:
  void begin();
  void setDuty(int _lamp,int _duty);
};
