#include "ros/ros.h"
#include "std_msgs/String.h"

void movementCallback(const std_msgs::String::ConstPtr& msg)
{
   //Do stuff when animal is detected
	//query CV service if necessary
}

int main(int argc, char **argv)
{
   //Initialize ROS node setup
   ros::init(argc, argv, "log");
   ros::NodeHandle n;

   ros::Subscriber sub = n.subscribe("Motor_Movement",10, movementCallback);

   while(ros::ok())
   {
      //Do stuff until someone presses ctrl-c
   }
   return 0;
}

