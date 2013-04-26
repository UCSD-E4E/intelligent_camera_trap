
class MotorController
{
	public:
	int pan_pos, tilt_pos, new_pan, new_tilt;

	MotorController(int pan_init, int tilt_init);

	int getPan(void);

	int getTilt(void);

	void updatePosition(void);

	void commandCallback(const std_msgs::String::ConstPtr& msg);

};

	
