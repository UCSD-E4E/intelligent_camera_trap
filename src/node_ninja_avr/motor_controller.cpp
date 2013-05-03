#include "motor_controller.h"
#define TOLERANCE = 0

void MotorController::updatePosition(void)
{
	if ((abs(new_pan - pan_pos) >= TOLERANCE) || (abs(new_tilt - tilt_pos) >= TOLERANCE)
		ROS_INFO("Setting pan, tilt to [%d,%d]", pan, tilt);
		pan_pos = pan;
		tilt_pos = tilt;
	else
		ROS_INFO("Change not big enough to move motors");
	return
}

/*void MotorController::commandCallback(const CamTrap_Viper::MotorCmd::ConstPtr& msg)
{
	new_pan = msg.new_x;
	new_tilt = msg.new_y;
	return
}*/

double MotorController::getPan(void)
{
	//update pan position: read motors?
	ROS_INFO("Current pan position: %d ", pan_pos);
}


double MotorController::getTilt(void)
{
	//update pan position: read motors?
	ROS_INFO("Current pan position: %d ", tilt_pos);
}


MotorController::MotorController(double pan_init, double tilt_init)
{
	pan_pos = pan_init;
	tilt_pos = tilt_init;
	
	new_pan = pan_pos;
	new_tilt = tilt_pos;
}
