#include "CamTrap_Viper/CvService.h"

class CVLocalizer
{
	public:
		int x, y, imgWidth, imgHeight; 
		double timestamp;
		double offset_const_x;
		double offset_const_y;
	
		CVLocalizer(int x_init, int y_init, const int imgW, const int imgH, const int FOV_X, const int FOV_Y);
		
		void updateLocation(int updated_x, int updated_y);

		void setX(int new_x);

		void setY(int new_y);
		
		double getTimsestamp(void);
		
		void setTimestamp(double new_timestamp);
		
		bool newCoords(CamTrap_Viper::CvService::Request &req, CamTrap_Viper::CvService::Response &res); 	


};



