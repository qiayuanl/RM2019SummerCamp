#include "../include/mainwindow.hpp"
#include <QApplication>
#include <ros/ros.h>

int main(int argc, char *argv[])
{
    std::map<std::string,std::string> remappings;

    remappings["__master"] = "http://10.21.59.73:11311";
    remappings["__hostname"] = "10.21.66.24";

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
