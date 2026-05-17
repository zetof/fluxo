#pragma once

// I2C infos for communications between controller and driver
#define I2C_DEV_ADDR 0x55 // Address used between master (controller) and slave (driver)

// Servomotor info
#define SERVO_CLOCK  4 // GPIO for servomotor clockwise rotation
#define SERVO_CCLOCK 5 // GPIO for servomotor counterclockwise rotation

// Circulator info
#define CIRCULATOR 18 // GPIO for circulator activation

// NTC10K thermo resistors
#define T1 32 // Thermo resistor 1
#define T2 33 // Thermo resistor 2
#define T3 34 // Thermo resistor 3
#define T4 35 // Thermo resistor 4
