#ifndef PORT_H_
#define PORT_H_

extern void PORT_Init();

extern unsigned char inputON1();
extern unsigned char inputON2();
extern unsigned char inputON3();
extern unsigned char inputON4();

extern void relayON1(unsigned char cmd);
extern void relayON2(unsigned char cmd);
extern void relayON3(unsigned char cmd);
extern void relayON4(unsigned char cmd);

#endif