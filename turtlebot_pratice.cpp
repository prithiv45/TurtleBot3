#include "ros/ros.h"
#include "cmath"

#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

class Turtlebot {
private:
    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub;
    
    float x_p, y_p, x_o, y_o, z_o, w_o;

public:
    Turtlebot() : n(), pub n.advertise<geometry_msgs::Twist>("/cmd_vel", 1),;
    
    sub(n.subscribe("/odom", 1, &Turtlebot::odomCallback)) {
        publishVelocity();
    }

    void odomCallback(const nav_msgs::Odometry::ConstPtr& data) {
        x_p = data->pose.pose.position.x;
        y_p = data->pose.pose.position.y;
        x_o = data->pose.pose.orientation.x;
        y_o = data->pose.pose.orientation.y;
        z_o = data->pose.pose.orientation.z;
        w_o = data->pose.pose.orientation.w;
    }
z
    float eulerFromQuaternion(float x, float y, float z, float w) {
        float t3 = +2.0 * (w * z + x * y);
        float t4 = +1.0 - 2.0 * (y * y + z * z);
        float yaw_z = std::atan2(t3, t4);
        float degree = yaw_z * (180.0 / M_PI);

        return (degree >= 0) ? degree : 360.0 + degree;
    }

    void publishVelocity() {
        float in_x = 0;
        float in_y = 0;
        float in_angle = 90.0;

        ros::Rate loop_rate(10);
        while (ros::ok()) {
            for (int i = 0; i < 4; i++) {
                while (true) {
                    float distance = sqrt(pow(x_p - in_x, 2) + pow(y_p - in_y, 2));
                    if (distance >= 1.0) {
                        ROS_INFO("FIRST WHILE IF");

                        stopRobot();
                        ros::spinOnce();
                        loop_rate.sleep();

                        while (true) {
                            if (isRobotAligned(in_angle)) {
                                ROS_INFO("SECOND WHILE IF");
                                stopRobot();
                                ros::spinOnce();
                                loop_rate.sleep();
                                break;
                            } else {
                                ROS_INFO("SECOND WHILE ELSE");
                                rotateRobot(0.2);
                                ros::spinOnce();
                                loop_rate.sleep();

                                if (i == 3 && (isRobotAtZeroOr360())) {
                                    ROS_INFO("new");
                                    in_angle = 0;
                                    stopRobot();
                                    ros::spinOnce();
                                    loop_rate.sleep();
                                    break;
                                } else {
                                    continue;
                                }
                            }
                            loop_rate.sleep();
                        }

                        in_x = x_p;
                        in_y = y_p;
                        in_angle = in_angle + 90.0;
                        break;
                    } else {
                        ROS_INFO("FIRST WHILE ELSE");
                        moveRobot(0.1);
                        ros::spinOnce();
                        loop_rate.sleep();
                    }
                }
            }
        }
    }

    void moveRobot(float linearVelocity) {
        geometry_msgs::Twist data;
        data.linear.x = linearVelocity;
        data.angular.z = 0.0;
        pub.publish(data);
    }

    void rotateRobot(float angularVelocity) {
        geometry_msgs::Twist data;
        data.linear.x = 0.0;
        data.angular.z = angularVelocity;
        pub.publish(data);
    }

    void stopRobot() {
        geometry_msgs::Twist data;
        data.linear.x = 0.0;
        data.angular.z = 0.0;
        pub.publish(data);
    }

    bool isRobotAligned(float targetAngle) {
        return eulerFromQuaternion(x_o, y_o, z_o, w_o) >= targetAngle &&
               eulerFromQuaternion(x_o, y_o, z_o, w_o) < (targetAngle + 2);
    }

    bool isRobotAtZeroOr360() {
        return eulerFromQuaternion(x_o, y_o, z_o, w_o) > 358 || eulerFromQuaternion(x_o, y_o, z_o, w_o) < 2;
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "draw");
    Turtlebot turtlebot;
    ros::spin();
    return 0;
}
