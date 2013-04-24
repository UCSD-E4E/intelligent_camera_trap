#include "ros.h"
#include "std_msgs/String.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

// Include C headers (ie, non C++ headers) in this block
extern "C"
{
  #include <util/delay.h>
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

ros::NodeHandle nh;

//Declare msgs, publishers, and subscribers here
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";
unsigned char cmd_off = 0;
unsigned char cmd_on = 1;

int main()
{
	PORTA = 0b00000000;
	DDRA = 0b00000000;
	PORTB = 0b00000000;
	DDRB = 0b00001111;		//set Relay as output (Bit0:3 = 1)
	PORTC = 0b00000000;
	DDRC = 0b00000000;
	PORTD = 0b00000000;
	DDRD = 0b00000010;		//set TX as output (Bit1 = 1)
  
	uint32_t lasttime = 0UL;
  	// Initialize ROS
  	nh.initNode();
  	nh.advertise(chatter);
	
	//Wait to connect to another ROS system,
	//Turn off relays once connected.
	PORTB = PORTB | 0b00001111;
	while (!nh.connected())
	{
		nh.spinOnce();
	} 
	PORTB = PORTB & 0b11110000;	
	
	/**IMPORTANT***/
	//-Must call nh.spinOnce() at least every few
	// seconds to maintain sync with main ROS 
	// system.
	/**************/	 
	
	while(1)
	{ 
		// Send the message every second
   	if(avr_time_now() - lasttime > 1000)
    	{
      	str_msg.data = hello;
      	chatter.publish(&str_msg);
      	lasttime = avr_time_now();
    	}
 		nh.spinOnce();	
	}
  return 0;
}
