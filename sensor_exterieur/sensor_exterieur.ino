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
#include "root_ca.h"
#include "secrets.h"
#include "setup.h"

bool wifiOK = false;
int wifiRetries;
float temp;
float humidity;
float pressure;

DHT dht22(DHT22_PIN, DHT22);
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Wire.begin(I2C_SDA,I2C_SCL);
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_MIN_FACTOR);

  while(!wifiOK) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi network");
    while(WiFi.status() != WL_CONNECTED && wifiRetries < WIFI_RETRIES) {
      delay(500);
      Serial.print(".");
      wifiRetries++;
    }
    if(wifiRetries < WIFI_RETRIES) wifiOK = true;
    else {
      Serial.println("");
      Serial.println("Problem connecting to WiFi. Turning off module, waiting for a while and retrying...");
      WiFi.disconnect(true);
      delay(1000 * WIFI_DELAY);
    }
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  dht22.begin();
  temp = dht22.readTemperature();
  humidity = dht22.readHumidity();

  if(!bmp.begin()) {
    Serial.println("No BMP180 module found, working without");
  }
  else {
    pressure = bmp.readPressure();
  }

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
  }
  else {
    if(isnan(pressure)) {
      Serial.println("Setting pressure to its default (0hPa)");
      pressure = 0;
    }
    else {
      pressure = pressure * (1 + (9.81 * ALTITUDE) / (287 * (temp + 273.15)));
    }

    Serial.println("Temperature: " + String(temp) + "°C");
    Serial.println("Humidity:    " + String(humidity) + "%");
    Serial.println("Pression:    " + String(pressure) + "hPa");

    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
      client->setCACert(RootCa);
      HTTPClient https;    
      if(https.begin(*client, API_ENDPOINT)) {
        https.addHeader("Content-Type", "application/json");
        https.addHeader("X-Api-Key", API_KEY);
        int httpResponseCode = https.POST("{\"id\":" + String(SENSOR_ID) + ",\"temperature\":" + String(temp) + ",\"humidity\":" + String(humidity) + ",\"pressure\":" + String(pressure / 100) +"}");
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
  Serial.println("Now Going in sleep mode for " + String(TIME_TO_SLEEP) + " minutes");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
}