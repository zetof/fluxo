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
#include "root_ca.h"
#include "secrets.h"

#define DHT22_PIN 15               // ESP32 pin GPIO15 connected to DHT22 sensor
#define uS_TO_S_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes
#define TIME_TO_SLEEP 10           // 10 minutes

const int altitude = 500;

float temp;
float humidity;
float pressure;

DHT dht22(DHT22_PIN, DHT22);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  delay(1000);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 10 minutes
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi network");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht22.begin();
  temp = dht22.readTemperature();
  humidity  = dht22.readHumidity();

  if(bmp.begin()) {
    pressure = bmp.readPressure();
  }

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
  }
  else {
    if(isnan(pressure)) {
      Serial.println("No BMP180 sensor found, setting pressure to default!");
      pressure = 0;
    }
    else {
      pressure = pressure * (1 + (9.81 * altitude) / (287 * (temp + 273.15)));
    }
    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
      client->setCACert(RootCa);
      HTTPClient https;    
      if(https.begin(*client, API_ENDPOINT)) {
        https.addHeader("Content-Type", "application/json");
        https.addHeader("X-Api-Key", API_KEY);
        int httpResponseCode = https.POST("{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humidity) + ",\"pressure\":" + String(pressure / 100) +"}");
        if(httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
        }
        else {
          Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
        }
      }
      https.end();
    }
  }
  Serial.println("Now Going in sleep mode for 10 minutes");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
}