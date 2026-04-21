#include <Rotary.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "setup.h"

const int rs = 13, en = 14, d4 = 9, d5 = 10, d6 = 11, d7 = 12, bl=15;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Rotary rotary(4, 5, 6);
 
void setup() {
  Serial.begin(9600);
  delay(1000);
  Wire.begin(I2C_SDA,I2C_SCL);
  rotary.begin();
  pinMode(bl, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("ROTARY SELECTOR:");
  digitalWrite(bl, LOW);
}
 
void loop() {
  int val = rotary.read();
  if(val != 0) {
    digitalWrite(bl, HIGH);
    lcd.setCursor(0, 1);
    if(val == 1) lcd.print("CLOCKWISE       ");
    else lcd.print("COUNTERCLOCKWISE");
    
    Wire.beginTransmission(I2C_DEV_ADDR);
    Wire.printf("ROTARY: %lu", val);
    uint8_t error = Wire.endTransmission(true);    
  }
  if(rotary.click()) {
    digitalWrite(bl, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("CLICK!          ");
    Serial.println("CLICK!");
  }
}