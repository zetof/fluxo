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

int timeToWakeUp = 0;
bool bmpActive = true;
bool balastActive = false;
int keepAliveDelay = BALAST_OFF * LOOP_DELAY;
float temp;
float humidity;
float pressure;

DHT dht22(DHT22_PIN, DHT22);
Adafruit_BMP085 bmp180;

void setup() {
  Serial.begin(9600);
  delay(1000);
  dht22.begin();
  Wire.begin(I2C_SDA,I2C_SCL);
  if(!bmp180.begin()) {
    Serial.println("No BMP180 module found, working without");
    bmpActive = false;
  }
  if(PB_KEEP_ALIVE) {
    Serial.println("Keep alive mode activated. You may use a powerbank to power your station");
    pinMode(BALAST_1_PIN, OUTPUT);
    pinMode(BALAST_2_PIN, OUTPUT);
    digitalWrite(BALAST_1_PIN, LOW);
    digitalWrite(BALAST_2_PIN, LOW);
  }
  else {
    Serial.println("Keep alive mode deactivated. Don't use a power bank or it will go idle");
  }
}

void loop() {
  if(timeToWakeUp == 0) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi network");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    temp = dht22.readTemperature();
    humidity = dht22.readHumidity();

    if(isnan(temp) || isnan(humidity)) {
      Serial.println("Failed to read from DHT22 sensor!");
    }
    else {
      if(bmpActive) {
        pressure = bmp180.readPressure();
      }
      else {
        Serial.println("Setting pressure to its default (0hPa)");
        pressure = 0;
      }
      pressure = pressure * (1 + (9.81 * ALTITUDE) / (287 * (temp + 273.15)));
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
    WiFi.disconnect(true);
    timeToWakeUp = TIME_TO_SLEEP * mS_TO_MIN_FACTOR;
  }
  delay(LOOP_DELAY);
  timeToWakeUp -= LOOP_DELAY;
  if(PB_KEEP_ALIVE) {
    keepAliveDelay -= LOOP_DELAY;
    if(keepAliveDelay == 0) {
      if(balastActive) {
        keepAliveDelay = BALAST_OFF * LOOP_DELAY;
        balastActive = false;
        digitalWrite(BALAST_1_PIN, LOW);
        digitalWrite(BALAST_2_PIN, LOW);
      }
      else {
        keepAliveDelay = BALAST_ON * LOOP_DELAY;
        balastActive = true;
        digitalWrite(BALAST_1_PIN, HIGH);
        digitalWrite(BALAST_2_PIN, HIGH);
      }
    }
  }
}