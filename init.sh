#!/bin/sh
modprobe can
modprobe can_raw
modprobe mttcan
ip link set can0 type can bitrate 1000000 dbitrate 1000000 berr-reporting on fd on
sudo ip link set up can0
