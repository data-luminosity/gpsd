#!/bin/bash

#scons --clean
scons build && adb forward tcp:4352 tcp:4352 && sudo ./gpsd -N -n -D5 tcp://localhost:4352
