#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <time.h>

#include <sys/statvfs.h>
#include "CamTrap_Viper/CvService.h"
#include "motor_controller.h"
#include <boost/asio.hpp>

//using namespace boost::asio;

void hvCallback(const std_msgs::String::ConstPtr& msg)
{
	//Do stuff when the hypervisor says so
}

void comCallback(const std_msgs::String::ConstPtr& msg)
{
	//Do stuff when the com node says so
}

int main(int argc, char** argv){
	ros::init(argc, argv, "motors");
	ros::NodeHandle n;
	ROS_INFO("Running Ninja Controller");
	//connect to Hypervisor and com nodes
	ros::Subscriber hv_sub = n.subscribe("Hypervisor_Output", 10, hvCallback);	
	ros::Subscriber com_sub = n.subscribe("Com_Commands", 10, comCallback);
	
	//Open connection for Log node to connect to
	ros::Publisher pub = n.advertise<std_msgs::String>("Motor_Movement",10);
	
	//Connect to CV service
	ros::ServiceClient client = n.serviceClient<CamTrap_Viper::CvService>("cv_service");
	CamTrap_Viper::CvService srv;

	MotorController mctrl("/dev/ttyUSB0", 57600, 0.0, 0.0);
	
	std::string response;
	
	
	while (ros::ok())
	{  
		response = mctrl.readPort();
		ROS_INFO("%s", response.c_str()); 
	 //Query new coordinates
     /* srv.request.localization_request = 0;
      if (client.call(srv))
      {   
       
      }   
      else
      {   
         ROS_ERROR("service call failed :(");
      }*/
	}
}
