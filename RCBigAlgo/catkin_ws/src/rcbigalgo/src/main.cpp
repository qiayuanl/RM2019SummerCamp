#include "../include/mainwindow.hpp"
#include <QApplication>
#include <ros/ros.h>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "rcbigalgo");
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
