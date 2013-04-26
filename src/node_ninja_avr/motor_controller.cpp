#include "motor_controller.h"

void MotorController::setPosisition(int pan, int tilt)
{
	ROS_INFO("Setting pan, tilt to [%d,%d]", pan, tilt);
	pan_pos = pan;
	tilt_pos = tilt;
	return
}

void MotorController::commandCallback(const CamTrap_Viper::MotorCmd::ConstPtr& msg)
{
	new_pan = msg.new_x;
	new_tilt = msg.new_y;
	return
}

int MotorController::getPan(void)
{
	//update pan position: read motors?
	ROS_INFO("Current pan position: %d ", pan_pos);
}


int MotorController::getTilt(void)
{
	//update pan position: read motors?
	ROS_INFO("Current pan position: %d ", tilt_pos);
}


MotorController::MotorController(int pan_init, int tilt_init)
{
	pan_pos = pan_init;
	tilt_pos = tilt_init;
	
	new_pan = pan_pos;
	new_tilt = tilt_pos;
}
