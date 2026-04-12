#include <Rotary.h>
 
Rotary rotary(36, 39, 34);
 
void setup() {
  Serial.begin(9600);
  delay(1000);
  rotary.begin();
}
 
void loop() {
  int val = rotary.read();
  if(val != 0) Serial.println(val);
  if(rotary.click()) Serial.println("CLICK!");
}