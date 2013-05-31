#ifndef PORT_H_
#define PORT_H_

extern void PORT_Init();


unsigned char panSensorInput();
unsigned char tiltSensorInput();
void hijack_EN(unsigned char cmd);
void tiltSensor_EN(unsigned char cmd);
void panCH1(unsigned char cmd);
void panCH2(unsigned char cmd);
void panCH3(unsigned char cmd);
void panCH4(unsigned char cmd);
void tiltCH1(unsigned char cmd);
void tiltCH2(unsigned char cmd);
void tiltCH3(unsigned char cmd);
void tiltCH4(unsigned char cmd);

#endif