#define	__AVR_ATmega16__	1
#define OSCSPEED	16000000		/* in Hz */
#include <string.h>
#include "HyperVisor.h"
#include "avr/io.h"
#define BAUD 57600
#define UBRR_VAL ((OSCSPEED / (16UL * BAUD)) - 1)


// Initialize the UART
void avr_uart_init(void)
{
  // Enable bidirectional UART
  UCSRB |= _BV(RXEN) | _BV(TXEN);
  // Use 8-bit characters
  //UCSRC |= _BV(UCSZ10) | _BV(UCSZ11);
   UCSRC = UCSRC | 0b10000110;
  // Set the Baud rate
  UBRRH = (UBRR_VAL >> 8);
  UBRRL = UBRR_VAL;
}


// Send one char (blocking)
void avr_uart_send_byte(uint8_t tx_byte)
{
  // Wait to be able to transmit
  while((UCSRA & _BV(UDRE)) == 0)
    asm volatile("nop"::);
  // Put the data into the send buffer
  UDR = tx_byte;
}


// Get one char if available, otherwise -1
int16_t avr_uart_receive_byte(void)
{
  if((UCSRA & 0b10000000) != 0)
  {
    return UDR;
  }
  else
  {
    return -1;
  }
}
