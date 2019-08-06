#!/bin/bash

sudo ~/jetson_clocks.sh
echo "================= FULL POWER ENABLED ================="
sleep 1

sudo ~/RM2019SummerCamp/Script/setupCAN.sh
echo "================= CAN INITIALIZED ================="
sleep 1

roslaunch ~/RM2019SummerCamp/Script/setupVision.launch &
sleep 5
renice -20 -p $!
echo "================= VISION ENABLED PID $! ================="


roslaunch ~/RM2019SummerCamp/Script/setupController.launch &
sleep 5
renice -15 -p $!
echo "================= CONTROLLER ENABLED PID $! ================="

