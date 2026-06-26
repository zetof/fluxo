/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-dht22
 */

#include <esp_sleep.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "Filter.h"
#include "root_ca.h"
#include "secrets.h"
#include "setup.h"

// Save sensor measures during sleep mode
RTC_DATA_ATTR float savedTemperature = 0; 
RTC_DATA_ATTR float savedHumidity = 0; 
RTC_DATA_ATTR float savedPressure = 0; 

// Intermediate readings for samples
float temp;
float humi;
float pres;

// Prepare median filters
Filter t;
Filter h;
Filter p;

// Function used to compute measured pressure at sea level
float seaLevelPressure(float readPressure, float temperature) {
    return readPressure * (1 + (9.81 * ALTITUDE) / (287 * (temperature + 273.15)));
}

// Function use to apply an exponential filter on previous and current values
float expFilter(float preValue, float value, float c) {
    if(preValue == 0) return value;
    else return (1 - c) * preValue + c * value; 
}

// Initialize temperature, humidity and pressure sensors
DHT dht22(DHT22_PIN, DHT22);
Adafruit_BMP085 bmp;

void setup() {

  // Start serail communication
  Serial.begin(9600);
  delay(1000);

  // Prepare wire lib for pressure sensor
  Wire.begin(I2C_SDA,I2C_SCL);
  
  // Prepare deep sleep mode
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_MIN_FACTOR);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi network");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Start reading temperature and humidity
  dht22.begin();
  
  // DHT22 is the minimum viable sensor. If not present abort reading stage
  if(isnan(t.addSample(dht22.readTemperature())) || isnan(h.addSample(dht22.readHumidity()))) Serial.println("Failed to read from DHT22 sensor, exiting!");
  else {

    // Read sensors with a defined amount of samples with a defined delay between each measure
    // We put each sample in appropriate filter for later computing of the median filter
    for(int i = 1; i < NB_OF_SAMPLES; i++) {  
        delay(1000 * TIME_BETWEEN_SAMPLES);
        temp = dht22.readTemperature();
        humi = dht22.readHumidity();
        t.addSample(temp);
        h.addSample(humi);
        if(bmp.begin()) pres = p.addSample(seaLevelPressure(bmp.readPressure(), temp));
        if(DEBUG) {
            Serial.println("=== SAMPLE: " + String(i) + " ===>");
            Serial.println("    * TEMP: " + String(temp) + "°C");
            Serial.println("    * HUMI: " + String(humi) + "%");
            Serial.println("    * PRES: " + String(pres) + "hPa");
        }
    }
    
    // We apply two filters: median filter and exponential filter with defined coefficients
    savedTemperature = expFilter(savedTemperature, t.medianFilter(), ALPHA_TEMP);
    savedHumidity = expFilter(savedHumidity, h.medianFilter(), ALPHA_HUMI);
    savedPressure = expFilter(savedPressure, p.medianFilter(), ALPHA_PRES);
    if(DEBUG) {
        Serial.println();
        Serial.println("=== FINAL MEASURES ===>");
        Serial.println("    * TEMP: " + String(savedTemperature) + "°C");
        Serial.println("    * HUMI: " + String(savedHumidity) + "%");
        Serial.println("    * PRES: " + String(savedPressure) + "hPa");
        Serial.println();
    }

    // We sent measured values to the database
    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
      client->setCACert(RootCa);
      HTTPClient https;    
      if(https.begin(*client, API_ENDPOINT)) {
        https.addHeader("Content-Type", "application/json");
        https.addHeader("X-Api-Key", API_KEY);
        int httpResponseCode = https.POST("{\"id\":" + String(SENSOR_ID) + ",\"temperature\":" + String(savedTemperature) + ",\"humidity\":" + String(savedHumidity) + ",\"pressure\":" + String(savedPressure / 100) +"}");
        if(DEBUG && httpResponseCode > 0) Serial.println("HTTP Response code: " + httpResponseCode);
        else {
          Serial.println();
          if(DEBUG) Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
        }
      }
      https.end();
    }
  }

  // Finally we go to deep sleep mode
  Serial.println("Now Going in sleep mode for " + String(TIME_TO_SLEEP) + " minutes");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
}
