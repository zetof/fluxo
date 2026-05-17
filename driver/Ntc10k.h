/*
  Ntc10k.h - Library to read temperature from an NTC10k Thermistor
  Created by ZeToF!, May 15, 2026
  Released into the public domain
*/
#ifndef Ntc10k_h

#define Ntc10k_h

#define R1 10000 // Resistor coupled to the thermistor to make the dividing bridge (in ohms)

/*
  Following coefficients have been estimated from the Thermistor Calculator at:
  https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html

  Temperature (°C) vs. Resistor (ohms)
  0    - 37700
  36.5 - 8550
  100  - 800
*/

#define C1 1.683147359e-03
#define C2 1.231236125e-04
#define C3 5.815507432e-07

#include "Arduino.h"

class Ntc10k
{
  public:
    Ntc10k(int analog_pin);
    float read();

  private:
    int _analogPin;
};

#endif
