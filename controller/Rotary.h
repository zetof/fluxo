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
    Rotary(int clk_pin, int dt_pin, int sw_pin);
    void begin();
    int read();
    bool click();

  private:
    int _clk_pin;
    int _dt_pin;
    int _sw_pin;
    int _counter = 0;
    bool _click = false;
    int _debounce = 0;
    const int _threshold = 100;
    void rotate();
    void button();
};

#endif