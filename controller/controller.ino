#include <Rotary.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "setup.h"

// Declare LCD 16x2 and rotary selector interfaces
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Rotary rotary(CLK_PIN, DT_PIN, SW_PIN);
 
void setup() {

  // Set serial speed for debugging and wait for a while before starting
  Serial.begin(9600);
  delay(1000);

  // Initialise I2C bus to slave driver device 
  Wire.begin(I2C_SDA,I2C_SCL);

  // Start LCD 16x2 and rotary selector
  lcd.begin(16, 2);
  rotary.begin();

  // Set GPIO mode for LCD 16x2 backlighting
  pinMode(BACKLIGHT, OUTPUT);
  lcd.print("ROTARY SELECTOR:");
  digitalWrite(BACKLIGHT, LOW);
}
 
void loop() {
  int val = rotary.read();
  if(val != 0) {
    digitalWrite(BACKLIGHT, HIGH);
    lcd.setCursor(0, 1);
    if(val == 1) lcd.print("CLOCKWISE       ");
    else lcd.print("COUNTERCLOCKWISE");
    
    Wire.beginTransmission(I2C_DEV_ADDR);
    Wire.printf("ROTARY: %lu", val);
    uint8_t error = Wire.endTransmission(true);    
  }
  if(rotary.click()) {
    digitalWrite(BACKLIGHT, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("CLICK!          ");
    Serial.println("CLICK!");
  }
}