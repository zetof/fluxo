#!/bin/bash

MICRO="esp32:esp32:esp32doit-devkit-v1"
USB="ttyUSB0"

if arduino-cli compile --fqbn $MICRO ./; then
    arduino-cli upload -p /dev/$USB --fqbn $MICRO ./
    arduino-cli monitor -p /dev/$USB --config 9600
fi
