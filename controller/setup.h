#pragma once

// I2C infos for communications between controller and driver
#define I2C_SDA      17   // GPIO for I2C communication SDA setup by wire lib
#define I2C_SCL      18   // GPIO for I2C communication SCL setup by wire lib
#define I2C_DEV_ADDR 0x55 // Address used between master (controller) and slave (driver)

// Rotary selector infos
#define CLK_PIN 4 // GPIO for rotary selector CLK pin
#define DT_PIN  5 // GPIO for rotary selector DT pin
#define SW_PIN  6 // GPIO for rotary selector SW pin

// LCD 16X2 infos
#define RS        13 // GPIO for LCD 16x2 RS pin
#define EN        14 // GPIO for LCD 16x2 EN pin
#define D4        9  // GPIO for LCD 16x2 D4 pin
#define D5        10 // GPIO for LCD 16x2 D5 pin
#define D6        11 // GPIO for LCD 16x2 D6 pin
#define D7        12 // GPIO for LCD 16x2 D7 pin
#define BACKLIGHT 15 // GPIO for LCD 16x2 D7 pin

// Servomotor infos
#define ROTATION_TIME 140 // Full 90 degress rotation time in seconds