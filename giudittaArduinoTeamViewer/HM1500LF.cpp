#ifndef HM1500LF_H
#include "HM1500LF.h"
#endif

#ifndef ARDUINO_H
#include <Arduino.h>
#endif

void HM1500LF::begin(int _pin)
{
  pin = _pin;
  pinMode(pin, INPUT);
}

float HM1500LF::getRHLinear()
{
  float _v = 5*analogRead(pin)/1.023; //[mV]
  return 0.03892*_v - 42.017;
}

float HM1500LF::getRHPolynomial()
{
  float _v = 5*analogRead(pin)/1.023; //[mV]
  return (float)(-1.91*pow(10,-9)*pow(_v,3)) + (float) (1.33*pow(10,-5)*pow(_v,2)) + (float)(9.56*pow(10,-3)*_v) - 20.16;
}

float HM1500LF::getRHCompensated(float _RH, float _T)
{
  return _RH + (_T- 23) * 0.05;
}
