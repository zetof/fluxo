#pragma once

// GPIO info
#define DHT22_PIN 15 // GPIO for DHT22 temperature and humidity sensot
#define I2C_SDA   9  // GPIO for I2C communication SDA setup by wire lib
#define I2C_SCL   8  // GPIO for I2C communication SCL setup by wire lib

// WiFi related info
#define WIFI_RETRIES 20 // Number of times we try to connect to WiFi befor disabling and restarting again
#define WIFI_DELAY   10 // Delay in seconds we turn off WiFi module in case of a problem 

// Sensors info
#define ALTITUDE 500 // Altitude of the sensor in meters

// Sensor ID
#define SENSOR_ID 2

// Sleep time between two measures
#define uS_TO_MIN_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes
#define TIME_TO_SLEEP    60          // 60 minutes (1 hour)