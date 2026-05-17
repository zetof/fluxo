#include <Wire.h>
#include <Servo.h>
#include <Ntc10k.h>
#include "setup.h"

uint32_t i = 0;

volatile bool cmd = false;

void onRequest() {
  Wire.print(i++);
  Wire.print(" Packets.");
  Serial.println("onRequest");
  Serial.println();
}

void onReceive(int len) {
  Serial.printf("onReceive[%d]: ", len);
  while (Wire.available()) {
    Serial.write(Wire.read());
  }
  Serial.println();
  cmd = true;
}

// Declare Servomotor
Servo servo(SERVO_CLOCK , SERVO_CCLOCK, SERVO_ROTATION_TIME);

// Declare temperature probes
Ntc10k t1(T1);
Ntc10k t2(T2);
Ntc10k t3(T3);
Ntc10k t4(T4);

void setup() {

  // Set serial speed for debugging and wait for a while before starting
  Serial.begin(9600);
  delay(1000);

  // Start Servomotor interface and set position in the middle
  servo.begin();
  servo.initialize();

  // Set relays GPIO to output mode
  pinMode(CIRCULATOR, OUTPUT);

  // By default nothing move
  digitalWrite(CIRCULATOR, HIGH);

  // Setup callbacks and I2C address for communication between controller and driver
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
}

void loop() {

  if(cmd) {
    Serial.println(String(t1.read(), 2));
    Serial.println(String(t2.read(), 2));
    cmd = false;
  }
  
  
  delay(100);
}
