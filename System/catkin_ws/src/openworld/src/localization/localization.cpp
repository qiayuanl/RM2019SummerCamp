#include <ros/ros.h>
#include "opencv2/opencv.hpp"
#include <algorithm>
using namespace std;
using namespace cv;

extern "C" {
#include "apriltag/apriltag.h"
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "localization");
	ros::NodeHandle nh;

    //////////////////////Paramters
    int p_VideoID;
    int p_FrameRate;

    nh.param<int>("VideoID", p_VideoID, 1);
    nh.param<int>("FrameRate", p_FrameRate, 60);
    //////////////////
    
    VideoCapture cap(p_VideoID);

    if (!cap.isOpened()) {
        cerr << "Couldn't open video capture device" << endl;
        return -1;
    }

    Mat frame;
    while (true) {
        cap >> frame;

        //Un-distortion
        Mat temp = frame.clone();
        //undistort(temp, frame, cameraMatrix, distCoeffs);

        imshow("Tag Detections", frame);
        if(waitKey(1000 / p_FrameRate) >= 0) break;
    }
}