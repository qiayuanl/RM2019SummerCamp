#!/bin/sh
ip link set can0 type can bitrate 1000000
sudo ip link set up can0

ip link set can1 type can bitrate 1000000
sudo ip link set up can1
