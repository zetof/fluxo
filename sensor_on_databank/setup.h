#pragma once

// Loop delay
#define LOOP_DELAY 1000 // Base unit of 1 second for the loop actions

// GPIO info
#define DHT22_PIN 15 // GPIO for DHT22 temperature and humidity sensot
#define I2C_SDA   8  // GPIO for I2C communication SDA setup by wire lib
#define I2C_SCL   9  // GPIO for I2C communication SCL setup by wire lib

// Senors info
#define ALTITUDE 500 // Altitude of the sensor in meters

// Sensor ID
#define SENSOR_ID 2

// Sleep time between two measures
#define mS_TO_MIN_FACTOR 60000 // Conversion factor for milliseconds to minutes
#define TIME_TO_SLEEP    60    // 60 minutes (1 hour)

// Power bank keep alive function
#define PB_KEEP_ALIVE true // Turn on/off the power bank keep alive functionallity
#define BALAST_OFF    10   // Interval in seconds to turn off balast
#define BALAST_ON     10   // Interval in seconds to turn on balast
#define BALAST_1_PIN  16   // First GPIO connected to a 100 ohms resistor
#define BALAST_2_PIN  17   // Second GPIO connected to a 100 ohms resistor