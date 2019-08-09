#include <ros/ros.h>
#include <std_msgs/Float64MultiArray.h>
#include <unistd.h>
#include <termios.h>

namespace MK_Blocking {
    namespace Servo {
        const int    MAX_SERVOS = 3;
        const double SERVO_DELAY = 1.0;

        ros::Publisher servo_pub;
        std::vector<double> angle;

        void update_angle() {
            std_msgs::Float64MultiArray data;

            data.data = angle;
            servo_pub.publish(data);

            ros::Duration(SERVO_DELAY).sleep();
        }

        void set_angle(int id, double val) {
            angle[id] = val;

            update_angle();
        }

        void init() {
            ros::NodeHandle nh;

            angle.resize(MAX_SERVOS);

	        servo_pub = nh.advertise<std_msgs::Float64MultiArray>("servo", 100);

            update_angle();
        }
    }

    namespace Ball {
        const int BALL_SERVO_ID = 0;
        const double BALL_DURATION = 0.3;

        int ball_cnt[3];
        const double ball_angle[3] = {0.48, 0.65, 0.82};

        void print() {
            ROS_INFO("Current Ball count: [%d %d %d]", ball_cnt[0], ball_cnt[1], ball_cnt[2]);
        }

        void init() {
            ball_cnt[0] = 4;
            ball_cnt[1] = 3;
            ball_cnt[2] = 4;

            print();
        }

        bool place_ball() {
            for(int idx = 0; idx < 3; idx++) if(ball_cnt[idx]) {
                Servo::set_angle(BALL_SERVO_ID, ball_angle[idx]);
                Servo::set_angle(BALL_SERVO_ID, 0.0);
                ros::Duration(BALL_DURATION).sleep();

                ball_cnt[idx]--;
                
                print();
                return true;
            }

            return false;
        }
    }

    void init_all() {
        Servo::init();

        Ball::init();
    }
}

ros::Publisher sigan_pub;

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

    sigan_pub = nh.advertise<std_msgs::Float64MultiArray>("motor", 100);
    std_msgs::Float64MultiArray sigan;
    sigan.data.resize(1);

    MK_Blocking::init_all();
	while (ros::ok())
	{
        int ch = getch();

        if(ch == 0x20) {
            MK_Blocking::Ball::place_ball();
        }
        else if(ch == 'w') {
            sigan.data[0] = -85.0;
            sigan_pub.publish(sigan);
        }
        else if(ch == 's') {
            sigan.data[0] = -5.0;
            sigan_pub.publish(sigan);
        }
	}
}
