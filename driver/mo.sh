#!/bin/bash

USB="ttyUSB0"

arduino-cli monitor -p /dev/$USB --config 9600
