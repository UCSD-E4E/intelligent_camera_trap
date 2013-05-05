#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"
#include "Atmega16Hardware.h"

namespace ros
{
  typedef ros::NodeHandle_<Atmega16Hardware, 5, 5, 100, 100> NodeHandle;
}

#endif
