/*
  Ntc10k.h - Library to read temperature from an NTC10k Thermistor
  Created by ZeToF!, May 15, 2026
  Released into the public domain
*/

#include <Arduino.h>
#include "Ntc10k.h"

Ntc10k::Ntc10k(int analogPin) {
  _analogPin = analogPin;
}

float Ntc10k::read() {
  int adc = analogRead(_analogPin);
  float r2 = R1 * (4095.0 / (float)adc - 1.0);
  float logR2 = log(r2);
  float temp = 1.0 / (C1 + C2 * logR2 + C3 * pow(logR2, 3)) - 273.15;
  return temp;
}
