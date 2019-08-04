#!/bin/bash

sudo ~/jetson_clocks.sh
echo "\n\n\n ================= FULL POWER ENABLED ================= \n\n\n"

sudo ~/RM2019SummerCamp/Script/setupCAN.sh
echo "\n\n\n ================= CAN INITIALIZED ================= \n\n\n"

roscore &
sleep 10
echo "\n\n\n ================= ROSCORE STARTED ================= \n\n\n"

rosrun rcbigcar robot &
echo "\n\n\n ================= MOTION ENABLED ================= \n\n\n"

roslaunch rcbigvis rcbigvis.launch &
echo "\n\n\n ================= VISION ENABLED ================= \n\n\n"
