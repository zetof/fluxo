/*
  Menu.h - Library to create and navigate a menu and take actions from events
  Created by ZeToF!, May 21, 2026
  Released into the public domain
*/
#ifndef Filter_h
#define Filter_h

#include "Arduino.h"

class Filter
{
  public:
    Filter();
    float getSample(int index);
    float addSample(float sample);
    float medianFilter();
    String samplesToString();

  private:
    float _samples[12];
    int _last = 0;
};

#endif
