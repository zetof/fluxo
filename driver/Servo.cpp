/*
  Servo.cpp - Library to control a 4 ways valve servomotor
  Created by ZeToF!, April 23, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include "Servo.h"

Servo::Servo(int clockwise_pin, int counterclockwise_pin, int full_90_degrees_time) {
  _clockwise_pin = clockwise_pin;
  _counterclockwise_pin = counterclockwise_pin;
  _full_90_degrees_time = full_90_degrees_time;
}

void Servo::begin() {
  pinMode(_clockwise_pin, OUTPUT);
  pinMode(_counterclockwise_pin, OUTPUT);
  Servo:stop();
}

void Servo::_turn(int time) {
  digitalWrite(_counterclockwise_pin, HIGH);
  delay(100);
  digitalWrite(_clockwise_pin, LOW);
}

void Servo::_counterclockwise(int time) {
  digitalWrite(_clockwise_pin, HIGH);
  delay(100);
  digitalWrite(_counterclockwise_pin, LOW);
}

void Servo::initialize() {
  Servo::_counterclockwise(_full_90_degrees_time);
  _position = 0;
  _status = COUNTERCLOCKWISE;
}

void Servo::set(int position) {
}

void Servo::stop() {
  _status = STOP;
  digitalWrite(_clockwise_pin, HIGH);
  digitalWrite(_counterclockwise_pin, HIGH);
}