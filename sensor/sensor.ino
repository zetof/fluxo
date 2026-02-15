/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-dht22
 */

#include <esp_sleep.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "root_ca.h"
#include "secrets.h"

#define DHT22_PIN  16 // ESP32 pin GPIO21 connected to DHT22 sensor

// Endpoint connection data
const char* endpoint = "https://fluxo.zetof.net/sensors";

DHT dht22(DHT22_PIN, DHT22);

void setup() {
  Serial.begin(9600);
  delay(1000);

  esp_sleep_enable_timer_wakeup(60000000); // 60 seconds
  
  dht22.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi network");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  float temp = dht22.readTemperature();
  float humidity  = dht22.readHumidity();
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    WiFiClientSecure *client = new WiFiClientSecure;
    if(client) {
      client->setCACert(RootCa);
      HTTPClient https;    
      if(https.begin(*client, endpoint)) {
        https.addHeader("Content-Type", "application/json");
        https.addHeader("X-Api-Key", API_KEY);
        int httpResponseCode = https.POST("{\"temperature\":" + String(temp) + ",\"humidity\":" + String(humidity) + "}");
        if(httpResponseCode > 0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
        }
        else {
          Serial.printf("[HTTPS] PUT... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
        }
      }
      https.end();
    }
  }
  Serial.println("Now Going in sleep mode for 1 minute");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
}