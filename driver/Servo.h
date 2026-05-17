/*
  Servo.h - Library to control a 4 ways valve servomotor
  Created by ZeToF!, April 23, 2026
  Released into the public domain
*/
#ifndef Servo_h

#define Servo_h
#define STOP 0
#define CLOCKWISE 1
#define COUNTERCLOCKWISE -1
#define SERVO_ROTATION_TIME 140

#include "Arduino.h"

class Servo
{
  public:
    Servo(int clockwise_pin, int counterclockwise_pin, int full_90_degrees_time);
    void begin();
    void initialize();
    void set(int position);
    void stop();
    int status();

  private:
    int _clockwisePin;
    int _counterclockwisePin;
    int _full90DegreesTime;
    int _position;
    bool _status = STOP;
    void _turn(int time);
    void _clockwise(int time);
    void _counterclockwise(int time);
};

#endif
