#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <time.h>

#include <sys/statvfs.h>

void animalCallback(const std_msgs::String::ConstPtr& msg)
{
	//Do stuff when animal is detected
}

int main(int argc, char **argv)
{
   //Initialize ROS node setup
   ros::init(argc, argv, "webcam");
   ros::NodeHandle n;

   ros::Subscriber sub = n.subscribe("Animal_Status",10, animalCallback);

   while(ros::ok)
   {
      //Do stuff until someone presses ctrl-c
   }
   return 0;
}

