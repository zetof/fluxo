/*
  Rotary.h - Library to reliably read a rotary selector with push button on en ESP32
  Created by ZeToF!, April 1, 2026
  Released into the public domain
*/
#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

class Rotary
{
  public:
    Rotary(int clkPin, int dtPin, int swPin);
    void begin();
    int read();
    bool click();

  private:
    int _clkPin;
    int _dtPin;
    int _swPin;
    int _counter = 0;
    bool _click = false;
    int _debounce = 0;
    const int _threshold = 100;
    void _rotate();
    void _button();
};

#endif
