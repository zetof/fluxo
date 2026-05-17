/*
  Servo.cpp - Library to control a 4 ways valve servomotor
  Created by ZeToF!, April 23, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include "Servo.h"

Servo::Servo(int clockwisePin, int counterclockwisePin, int full90DegreesTime) {
  _clockwisePin = clockwisePin;
  _counterclockwisePin = counterclockwisePin;
  _full90DegreesTime = full90DegreesTime;
}

void Servo::begin() {
  pinMode(_clockwisePin, OUTPUT);
  pinMode(_counterclockwisePin, OUTPUT);
  Servo:stop();
}

void Servo::_turn(int time) {
  digitalWrite(_counterclockwisePin, HIGH);
  delay(100);
  digitalWrite(_clockwisePin, LOW);
}

void Servo::_counterclockwise(int time) {
  digitalWrite(_clockwisePin, HIGH);
  delay(100);
  digitalWrite(_counterclockwisePin, LOW);
}

void Servo::initialize() {
  Servo::_counterclockwise(_full90DegreesTime);
  _position = 0;
  _status = COUNTERCLOCKWISE;
}

void Servo::set(int position) {
}

void Servo::stop() {
  _status = STOP;
  digitalWrite(_clockwisePin, HIGH);
  digitalWrite(_counterclockwisePin, HIGH);
}
