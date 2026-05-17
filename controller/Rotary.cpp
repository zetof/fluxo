/*
  Rotary.cpp - Library to reliably read a rotary selector with push button on en ESP32
  Heavily base on this page: https://garrysblog.com/2021/03/20/reliably-debouncing-rotary-encoders-with-arduino-and-esp32/
  Created by ZeToF!, April 1, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include "Rotary.h"

Rotary::Rotary(int clkPin, int dtPin, int swPin) {
  _clkPin = clkPin;
  _dtPin = dtPin;
  _swPin = swPin;
}

void Rotary::begin() {
  pinMode(_clkPin, INPUT);
  pinMode(_dtPin, INPUT);
  pinMode(_swPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(_clkPin), std::bind(&Rotary::_rotate, this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(_dtPin), std::bind(&Rotary::_rotate, this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(_swPin), std::bind(&Rotary::_button, this), RISING);
}

void Rotary::_rotate() {  
  static uint8_t oldAB = 3;
  static int8_t encval = 0;  
  static const int8_t encStates[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
 
  oldAB <<=2;  
 
  if(digitalRead(_clkPin)) oldAB |= 0x02;
  if(digitalRead(_dtPin)) oldAB |= 0x01;
   
  encval += encStates[( oldAB & 0x0f )];
 
  if(encval > 3) {
    _counter++;
    encval = 0;
  }
  else if(encval < -3) {
   _counter--;
   encval = 0;
  }
}

void Rotary::_button() {
  if(millis() - _debounce > _threshold) {
    _debounce = millis();
    _click = true;
  }
}

int Rotary::read() {
  int val = _counter;
  _counter = 0;
  return val;
}

bool Rotary::click() {
  bool val = _click;
  _click = false;
  return val;

}
