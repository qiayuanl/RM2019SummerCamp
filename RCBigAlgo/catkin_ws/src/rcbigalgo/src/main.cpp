#include "../include/mainwindow.hpp"
#include <QApplication>
#include <ros/ros.h>

int main(int argc, char *argv[])
{
    std::map<std::string,std::string> remappings;

    remappings["__master"] = "http://192.168.43.53:11311";
    remappings["__hostname"] = "192.168.43.16";

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
