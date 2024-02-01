#include "ros/ros.h"
#include "cmath"

#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

geometry_msgs::Twist msg;
ros::Publisher pub;

float x_cp,y_cp,x_co,y_co,z_co,w_co;



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
    

void odom_cb(const nav_msgs::Odometry::ConstPtr& msg)
{
  x_cp = msg->pose.pose.position.x;
  y_cp = msg->pose.pose.position.y;
  
  x_co = msg->pose.pose.orientation.x;
  y_co = msg->pose.pose.orientation.y;
  z_co = msg->pose.pose.orientation.z;
  w_co = msg->pose.pose.orientation.w;
  
 

}

void publish_vel()
{

  float ini_x = 0;
  float ini_y = 0;

  float ini_angle = 90.0;

  ros::Rate loop_rate(10);
  while (ros::ok())
  {

    for(int i = 0;i<4;i++){

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
               if(euler_from_quaternion(x_co,y_co,z_co,w_co)>= ini_angle){
                  ROS_INFO("SECOND WHILE IF");
                  msg.angular.z = 0.0; 
                  msg.linear.x = 0.0;
                  pub.publish(msg);
                  ros::spinOnce();
                  break;
              }
               else{
                ROS_INFO("SECOND WHILE ELSE");
                msg.angular.z = 0.1; 
                msg.linear.x = 0.0;
                pub.publish(msg);
                ros::spinOnce();
                loop_rate.sleep(); 
              }

              
            }

            ini_x = x_cp;
            ini_y = y_cp;
            ini_angle = ini_angle + 90;

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

    

    

  }

}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "draw_square");
  ros::NodeHandle n;    
  

  pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  ros::Subscriber sub = n.subscribe("/odom",10,odom_cb);
  // 

  publish_vel();

  // ros::spin();

  return 0;
}

