#include "../include/mainwindow.hpp"
#include <QApplication>
#include <ros/ros.h>

int main(int argc, char *argv[])
{
    std::map<std::string,std::string> remappings;
    if(argc >= 2) remappings["__master"] = argv[1]; //"http://192.168.10.164:11311";
    if(argc >= 3) remappings["__hostname"] = argv[2]; //"192.168.10.131";
    ros::init(remappings, "rcbigalgo");

    ros::start();

    ros::AsyncSpinner spinner(1);
    spinner.start();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int ret = a.exec();

    if(ros::isStarted()) {
        spinner.stop();

        ros::shutdown(); // explicitly needed since we use ros::start();
        ros::waitForShutdown();
    }

    return ret;
}
