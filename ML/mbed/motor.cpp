#include "motor.h"

char packetNum = 11;
char commandCharNum = 1;

char commandPan[15][6] =
{"R0120", "R0100", "R0060", "R0040",
 "R0020", "R0010", "R0005", "R0000",
 "L0005", "L0010", "L0020", "L0040",
 "L0060", "L0100", "L0120"};

char commandTilt[3][6] =
{"U0020", "Z0000", "D0020"};

char commandCenter[11] = { "X0500Y0200" };

MOTOR::MOTOR(Serial* motorUart)
{
    motorUartControl = motorUart;
    motorUartControl->baud(19200);
}

void MOTOR::sendCommand(unsigned int trackRow, unsigned int totalRow,unsigned int trackCol, unsigned int totalCol)
{
  //TODO: Normalize
  motorUartControl->printf("STD%c%s%sAA",packetNum,commandPan[trackCol], commandTilt[trackRow]);

}

void MOTOR::center(void)
{
  motorUartControl->printf("STC%ccAASTC%cdAA",commandCharNum,commandCharNum);
}


