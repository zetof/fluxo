/*
  Rotary.cpp - Library to reliably read a rotary selector with push button on en ESP32
  Heavily base on this page: https://garrysblog.com/2021/03/20/reliably-debouncing-rotary-encoders-with-arduino-and-esp32/
  Created by ZeToF!, April 1, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include "Rotary.h"

Rotary::Rotary(int clk_pin, int dt_pin, int sw_pin) {
  _clk_pin = clk_pin;
  _dt_pin = dt_pin;
  _sw_pin = sw_pin;
}

void Rotary::begin() {
  pinMode(_clk_pin, INPUT);
  pinMode(_dt_pin, INPUT);
  pinMode(_sw_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(_clk_pin), std::bind(&Rotary::rotate, this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(_dt_pin), std::bind(&Rotary::rotate, this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(_sw_pin), std::bind(&Rotary::button, this), RISING);
}

void Rotary::rotate() {  
  static uint8_t old_AB = 3;
  static int8_t encval = 0;  
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
 
  old_AB <<=2;  
 
  if(digitalRead(_clk_pin)) old_AB |= 0x02;
  if(digitalRead(_dt_pin)) old_AB |= 0x01;
   
  encval += enc_states[( old_AB & 0x0f )];
 
  if(encval > 3) {
    _counter++;
    encval = 0;
  }
  else if(encval < -3) {
   _counter--;
   encval = 0;
  }
}

void Rotary::button() {
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