#!/bin/bash

USB="ttyACM0"

arduino-cli monitor -p /dev/$USB --config 9600
