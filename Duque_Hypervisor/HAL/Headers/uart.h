#ifndef UART_H_
#define UART_H_

extern void UART_Init(uint32_t Baud);
extern unsigned char UART_Receive();
extern unsigned char UART_ReceiveSuccess(char* data);
extern void UART_Transmit(unsigned char Data);

#endif