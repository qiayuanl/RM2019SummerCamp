#!/bin/bash

sudo ~/jetson_clocks.sh
echo "================= FULL POWER ENABLED ================="
sleep 1

sudo ~/RM2019SummerCamp/Script/setupCAN.sh
echo "================= CAN INITIALIZED ================="
sleep 1

roslaunch ~/RM2019SummerCamp/Script/setupNodes.launch