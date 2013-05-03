#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "ros/ros.h"

#include <boost/asio.hpp>

//using namespace boost::asio;

class MotorController
{
	public:
		double pan_pos, tilt_pos, new_pan, new_tilt;
	
	MotorController(std::string port, unsigned int baudrate, double pan_init, double tilt_init) : io(), serial(io, port){
		pan_pos = pan_init;
		tilt_pos = tilt_init;

		new_pan = pan_pos;
		new_tilt = tilt_pos;

		serial.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
	}

	void getPanTilt();

	void updatePosition();

//	void commandCallback(const std_msgs::String::ConstPtr& msg);
	
	std::string readPort();

	int sendSteps(int deg_x, int deg_y);

	private:
		boost::asio::io_service io;
		boost::asio::serial_port serial;
};
