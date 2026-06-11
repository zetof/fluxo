/*
  Filter.h - Library to filter and eliminate noise from measures taken by sensors
  Created by ZeToF!, May R2117, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include "Filter.h"

Filter::Filter() {
    memset(_samples, 0, sizeof(_samples));
}

float Filter::getSample(int index) {
    return _samples[index];
}

float Filter::addSample(float sample) {
    if(_last == 0) _samples[0] = sample;
    else {
        int i = _last - 1;
        while(i >= 0 && _samples[i] > sample) {
            _samples[i + 1] = _samples[i];
            i--;
        }
        _samples[i + 1] = sample;
    }
    _last++;
    return sample;
}

float Filter::medianFilter() {
    float median;
    if(_last % 2 == 0) median = (_samples[_last / 2 - 1] + _samples[_last / 2]) / 2;
    else median = _samples[(_last + 1) / 2 - 1];
    return median;
}

String Filter::samplesToString() {
    String result = "[ ";
    for(int i = 0; i < _last; i++) {
        if(i != 0) result += ", ";
        result += String(_samples[i]);
    }
    result += " ]";
    return result;
}
