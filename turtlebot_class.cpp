#include "ros/ros.h"
#include "cmath"

#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

float x_cp,y_cp,x_co,y_co,z_co,w_co;


void odom_cb(const nav_msgs::Odometry::ConstPtr& msg)
{
    x_cp = msg->pose.pose.position.x;
    y_cp = msg->pose.pose.position.y;
    
    x_co = msg->pose.pose.orientation.x;
    y_co = msg->pose.pose.orientation.y;
    z_co = msg->pose.pose.orientation.z;
    w_co = msg->pose.pose.orientation.w;
    
}



class Square{
    public:
    geometry_msgs::Twist msg;
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    ros::Subscriber sub = n.subscribe("/odom",1,odom_cb);

    

    Square(){
        publish_vel();
    }

    float euler_from_quaternion(float x, float y, float z, float w)
    {
        float t3 = +2.0 * (w*z+x*y);
        float t4 = +1.0 - 2.0 * (y*y+z*z);
        float yaw_z = std::atan2(t3,t4);

        float degree = yaw_z * (180.0 / M_PI);

        if (degree>=0)
        {
            return degree;
        } 
        else{
            float new_degree = 360.0 + degree;
            return new_degree;
        }
    }



    void publish_vel()
    {

    float ini_x = 0;
    float ini_y = 0;

    float ini_angle = 90.0;

    ros::Rate loop_rate(10);
    while (ros::ok())
    {

        for(int i = 0;i<3;i++){

            while (true)
            {
            float distance = sqrt(pow(x_cp-ini_x,2)+pow(y_cp-ini_y,2));
            if(distance >= 1.0){

                ROS_INFO("FIRST WHILE IF");

                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
                pub.publish(msg);
                ros::spinOnce();
                loop_rate.sleep(); 

                while (true)
                {
                if(euler_from_quaternion(x_co,y_co,z_co,w_co)>= ini_angle && euler_from_quaternion(x_co,y_co,z_co,w_co) < (ini_angle+2)){
                    ROS_INFO("SECOND WHILE IF");
                    msg.angular.z = 0.0; 
                    msg.linear.x = 0.0;
                    pub.publish(msg);
                    ros::spinOnce();

                    loop_rate.sleep();
                    break;
                }
                else{
                    ROS_INFO("SECOND WHILE ELSE");
                    msg.angular.z = 0.2; 
                    msg.linear.x = 0.0;
                    pub.publish(msg);
                    ros::spinOnce();
                    loop_rate.sleep();

                    if(i==3 && (euler_from_quaternion(x_co,y_co,z_co,w_co) >358 || euler_from_quaternion(x_co,y_co,z_co,w_co) < 2))
                    {


                    ROS_INFO("newww");
                    msg.angular.z = 0.0; 
                    msg.linear.x = 0.0;
                    pub.publish(msg);
                    ros::spinOnce();

                    loop_rate.sleep();
                    break;

                    }
                    else
                    {
                    continue;
                    }
                }
                loop_rate.sleep();

                
                }

                ini_x = x_cp;
                ini_y = y_cp;
                ini_angle = ini_angle + 90.0;

                break;
                

            }

            else
            {
            ROS_INFO("FIRST WHILE ELSE");
            msg.linear.x = 0.1;
            msg.angular.z = 0.0;
            pub.publish(msg);
            ros::spinOnce();
            loop_rate.sleep();
    
            }

            }

        }
        ini_angle = 0;
        ROS_INFO("FIRST WHILE ELSE");
    }

    }



};


int main(int argc, char **argv){
    ros::init(argc, argv, "square");
    Square obj;
}



    