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
echo "Vision PID $!"
sleep 5
sudo renice -20 -p $(./findchildren.sh $!)
sudo chrt -r -p 50 $(./findchildren.sh $!)


echo "================= VISION ENABLED ================="


roslaunch ~/RM2019SummerCamp/Script/setupController.launch &
echo "Controller PID $!"
sleep 5
sudo renice -15 -p $(./findchildren.sh $!)
sudo chrt -r -p 50 $(./findchildren.sh $!)

echo "================= CONTROLLER ENABLED ================="

# echo "############################"
# echo "############################"
# echo "####                    ####"
# echo "############################"
# echo "############################"

while true ; do 
   sleep 1
done


<<<<<<< HEAD
roslaunch ~/RM2019SummerCamp/Script/setupController.launch

roslaunch ~/RM2019SummerCamp/Script/setupVision.launch
=======
# sleep 365d
# pstree pid -p| awk -F"[()]" '{print $2}'| xargs kill -9
>>>>>>> 5e20553e82422694b3728e7c828464ac77f54eeb
