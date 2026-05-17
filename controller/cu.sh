#!/bin/bash

MICRO="esp32:esp32:esp32s3"
USB="ttyACM0"

if arduino-cli compile --fqbn $MICRO ./; then
    arduino-cli upload -p /dev/$USB --fqbn $MICRO ./
    arduino-cli monitor -p /dev/$USB --config 9600
fi
