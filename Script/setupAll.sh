#!/bin/bash

control_c() {
    kill -TERM $$
    # exit
}

trap control_c SIGINT

sudo ~/jetson_clocks.sh
echo "================= FULL POWER ENABLED ================="
sleep 2

sudo ~/RM2019SummerCamp/Script/setupCAN.sh
echo "================= CAN INITIALIZED ================="
sleep 2



# TODO get children pid and set nice priority

roslaunch ~/RM2019SummerCamp/Script/setupVision.launch 2> /dev/null &
VISION_PID=$!
sleep 5
sudo chrt -r --pid 50 $(./findchildrenpid.sh $VISION_PID)
sudo renice -20 -p $(./findchildrenpid.sh $VISION_PID)

echo "Vision PID $VISION_PID"
echo "================= VISION ENABLED ================="


roslaunch ~/RM2019SummerCamp/Script/setupController.launch &
CONTROLLER_PID=$!
sleep 5
sudo chrt -r --pid 40 $(./findchildrenpid.sh $CONTROLLER_PID)
sudo renice -15 -p $(./findchildrenpid.sh $CONTROLLER_PID)

echo "Controller PID $CONTROLLER_PID"
echo "================= CONTROLLER ENABLED ================="

# echo "############################"
# echo "############################"
# echo "####                    ####"
# echo "############################"
# echo "############################"

# while true ; do 
#    sleep 1
# done

top

# sleep 365d
# pstree pid -p| awk -F"[()]" '{print $2}'| xargs kill -9
