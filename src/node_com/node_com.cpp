#include "ros/ros.h"
#include "std_msgs/String.h"

/*void movementCallback()
{
   //Do stuff when animal is detected
	//query CV service if necessary
}*/

int main(int argc, char **argv)
{
   //Initialize ROS node setup
   ros::init(argc, argv, "com");
   ros::NodeHandle n;

   ros::Publisher pub = n.advertise<std_msgs::String>("Com_Commands",10);

   while(ros::ok)
   {
      //Do stuff until someone presses ctrl-c
   }
   return 0;
}

