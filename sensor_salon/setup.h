#pragma once

// GPIO info
#define DHT22_PIN 15 // GPIO for DHT22 temperature and humidity sensot
#define I2C_SDA   21 // GPIO for I2C communication SDA setup by wire lib
#define I2C_SCL   22 // GPIO for I2C communication SCL setup by wire lib

// Senors info
#define ALTITUDE 500 // Altitude of the sensor in meters

// Sleep time between two measures
#define uS_TO_S_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes
#define TIME_TO_SLEEP  60          // 60 minutes (1 hour)