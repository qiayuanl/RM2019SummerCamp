#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <unistd.h>
#include <termios.h>

ros::Publisher ball_pub;

// For non-blocking keyboard inputs
int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "mecanics");

    ros::NodeHandle nh;

    //Begin Spinning
    ros::AsyncSpinner spinner(1);
    spinner.start();

    ball_pub = nh.advertise<std_msgs::Float64MultiArray>("motor", 100);
    std_msgs::Float64MultiArray ball;
    ball.data.resize(3);

	  while (ros::ok())
	  {
        int ch = getch();

        if(ch == 0x20) {
            ball.data[0] = 0;
            ball.data[1] = -1.7;
            ball.data[2] = 1.7;
            ball_pub.publish(ball);

            usleep(1000 * 1000);

            ball.data[0] = 0;
            ball.data[1] = -2.7;
            ball.data[2] = 2.7;
            ball_pub.publish(ball);

            usleep(1000 * 1000);

            printf("Placed\n");
        }
	  }
}
