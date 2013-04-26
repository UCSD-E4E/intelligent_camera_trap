#ifndef _AVR_UART_H_
#define _AVR_UART_H_
/*
extern void UART_Init(uint32_t Baud);
extern unsigned char UART_Receive();
extern unsigned char UART_ReceiveSuccess(char* data);
extern void UART_Transmit(unsigned char Data);
*/


void avr_uart_init(void);
void avr_uart_send_byte(uint8_t tx_byte);
int16_t avr_uart_receive_byte(void);

#endif
