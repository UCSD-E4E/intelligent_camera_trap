#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "mbed.h"

class MOTOR {
public:
  MOTOR(Serial* motorUart);
  void sendCommand(unsigned int trackRow, unsigned int totalRow,unsigned int trackCol, unsigned int totalCol);
  void center(void);
    
private:
  Serial* motorUartControl;
};



#endif
