#include <sstream>
#include <iostream>
#include <time.h>
#include "std_msgs/String.h"

using namespace std;

int getHour(string timeString);
int getMin(string timeString);
int getSec(string timeString);
int getMonth(string timeString);
int getDay(string timeString);
int getYear(string timeString);

int getHour(string timeString){
	int hour = (timeString[0] - 48)*10 + (timeString[1] - 48);
	//cout << "hour = " << hour << "\n"; 
	return hour;
}


int getMin(string timeString){
	int min = (timeString[3] - 48)*10 + (timeString[4] - 48);
	//cout << "min = " << min << "\n"; 
	return min;
}

int getSec(string timeString){
	int sec = (timeString[6] - 48)*10 + (timeString[7] - 48);
	//cout << "sec = " << sec << "\n"; 
	return sec;
}


int getMonth(string timeString){
	int month = (timeString[9] - 48)*10 + (timeString[10] - 48);
	//cout << "month = " << month << "\n"; 
	return month;
}


int getDay(string timeString){
	int day = (timeString[12] - 48)*10 + (timeString[13] - 48);
	//cout << "day = " << day << "\n"; 
	return day;
}


int getYear(string timeString){
	int year = (timeString[15] - 48)*1000 + (timeString[16] - 48)*100 + (timeString[17]-48)*10 +(timeString[18] - 48);
	//cout << "year = " << year << "\n"; 
	return year;
}
