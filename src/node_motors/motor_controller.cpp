#include "motor_controller.h"
#define TOLERANCE 0.0
#define PAN_STEPS 883.0
#define PAN_RANGE 180
#define TILT_STEPS 355
#define TILT_RANGE 72

using namespace boost::asio;

void MotorController::updatePosition()
{
	//update current motor position
	//getPanTilt();
/*	if ((abs(new_pan - pan_pos) > TOLERANCE) || (abs(new_tilt - tilt_pos) > TOLERANCE))
	{
		ROS_INFO("Setting pan, tilt to [%f,%f]", new_pan, new_tilt);
		int pan_steps = (new_pan-pan_pos)*(PAN_STEPS/PAN_RANGE);
		
		//send to motor controller
		sendSteps(pan_steps, 0);
	}
	else
	{
		ROS_INFO("Change not big enough to move motors");
	}
	return;*/
	sendSteps(100,100);
}

/*void MotorController::commandCallback(const CamTrap_Viper::MotorCmd::ConstPtr& msg)
{
	new_pan = msg.new_x;
	new_tilt = msg.new_y;
	return
}*/

void MotorController::getPanTilt()
{
	//update pan/tilt position: get steps
/* 	char query[7];
 	sprintf(query, "STQ%c9A", 1);
	char response[13];
	int res_length = read(fd, response, 13);
	int steps_x = -1;
	int steps_y = -1;
	
	sscanf(response, "X:%dY:%d", steps_x, steps_y);
	
	double degrees_x = steps_x * (PAN_RANGE/PAN_STEPS);
	double degrees_y = steps_y * (TILT_RANGE/TILT_STEPS);
	ROS_INFO("Current pan position: %f", degrees_x);
	ROS_INFO("Current tilt position: %f", degrees_y);
	return;*/
}


/*double MotorController::getTilt(void)
{
	//update pan position: read motors?
	double degrees = 0;
	ROS_INFO("Current pan position: %f", degrees);
	return degrees;
}*/

std::string MotorController::readPort()
{
	using namespace boost;
	char c;
	std::string result;
	for(;;)
	{
		asio::read(serial, asio::buffer(&c, 1));
		
		switch(c)
		{
			case '\r':
				break;
			case '\n':
				return result;
			default:
				result+=c;
		}
	}
}

int MotorController::sendSteps(int steps_x, int steps_y)
{
	//construct message
	unsigned char checksum = 'A';
	char* packet = (char *)malloc(snprintf(NULL, 0, "X0%dY0%d", steps_x, steps_y) + 1);
	char char_count = sprintf(packet, "X0%dY0%d", steps_x, steps_y);
	
	char* cmd = (char *)malloc(snprintf(NULL, 0, "STD%c%sA", char_count, packet) + 1);
	int length = sprintf(cmd, "STC%c%sA", char_count, packet);
	
	
	ROS_INFO("Wrote to /dev/ttyUSB0: %s", cmd);
	ROS_INFO("Steps = [%d, %d]", steps_x, steps_y); 
	return 0;
}
	
//}

/*MotorController::MotorController(std::string port, unsigned int baudrate, double pan_init, double tilt_init) : io(), serial(io, port)
{
	pan_pos = pan_init;
	tilt_pos = tilt_init;
	
	new_pan = pan_pos;
	new_tilt = tilt_pos;
	
	serial.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
	
}*/
