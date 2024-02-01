#include "ros/ros.h"
#include "cmath"

#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

float x_p,y_p,x_o,y_o,z_o,w_o;


void odom_callback(const nav_msgs::Odometry::ConstPtr& data)
{
    x_p = data->pose.pose.position.x;
    y_p = data->pose.pose.position.y;
    
    x_o = data->pose.pose.orientation.x;
    y_o = data->pose.pose.orientation.y;
    z_o = data->pose.pose.orientation.z;
    w_o = data->pose.pose.orientation.w;
    
}

class turtlebot{
    public:
    geometry_msgs::Twist data;
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    ros::Subscriber sub = n.subscribe("/odom",1,odom_callback);

    

    turtlebot(){
        publish_velocity();
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



    void publish_velocity()
    {

    float in_x = 0;
    float in_y = 0;

    float in_angle = 90.0;

    ros::Rate loop_rate(10);
    while (ros::ok())
    {

        for(int i = 0;i<4;i++){

            while (true)
            {
            float distance = sqrt(pow(x_p-in_x,2)+pow(y_p-in_y,2));
            if(distance >= 1.0){

                ROS_INFO("FIRST WHILE IF");

                data.linear.x = 0.0;
                data.angular.z = 0.0;
                pub.publish(data);
                ros::spinOnce();
                loop_rate.sleep(); 

                while (true)
                {
                if(euler_from_quaternion(x_o,y_o,z_o,w_o)>= in_angle && euler_from_quaternion(x_o,y_o,z_o,w_o) < (in_angle+2)){
                    ROS_INFO("SECOND WHILE IF");
                    data.angular.z = 0.0; 
                    data.linear.x = 0.0;
                    pub.publish(data);
                    ros::spinOnce();

                    loop_rate.sleep();
                    break;
                }
                else{
                    ROS_INFO("SECOND WHILE ELSE");
                    data.angular.z = 0.2; 
                    data.linear.x = 0.0;
                    pub.publish(data);
                    ros::spinOnce();
                    loop_rate.sleep();

                    if(i==3 && (euler_from_quaternion(x_o,y_o,z_o,w_o) >358 || euler_from_quaternion(x_o,y_o,z_o,w_o) < 2))
                    {


                    ROS_INFO("new");
                    in_angle = 0;
                    data.angular.z = 0.0; 
                    data.linear.x = 0.0;
                    pub.publish(data);
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

                in_x = x_p;
                in_y = y_p;
                in_angle = in_angle + 90.0;

                break;
                

            }

            else
            {
            ROS_INFO("FIRST WHILE ELSE");
            data.linear.x = 0.1;
            data.angular.z = 0.0;
            pub.publish(data);
            ros::spinOnce();
            loop_rate.sleep();
    
            }

            }

        }
        //in_angle = 0;
        // ROS_INFO("FIRST WHILE ELSE");
    }

    }



};


int main(int argc,char**argv){
    ros::init(argc,argv,"draw");
    turtlebot obj;

} 
