#ifndef __E_UART__
#define __E_UART__
#include <stdint.h>
#include <inavr.h>
#include <ioavr.h>
#define F_OSC 16000000UL
#define BAUD 9600
#define BAUD_RATE (F_OSC / 16 / BAUD - 1)
#define TRANSMITTER (1 << TXEN3)
#define RECEIVER (1 << RXEN3)
#define TXD_OUT() DDRJ |= (1 << PJ1)
#define RXD_IN() DDRJ &= ~(1 << PJ0)



void usart_initialize(uint16_t baud_rate);
// Funcția de transmitere USART
void usart_transmit(uint8_t data);
// Funcția de recepție UART
uint8_t usart_receive(void);
#endif

#define TX_BUF_LEN 256
#define RX_BUF_LEN 256

uint16_t get_next_idx(uint16_t current_idx, uint16_t buflen);


void put_uart_rx(uint8_t data);
uint8_t get_uart_rx();


void put_uart_tx(uint8_t data);
uint8_t get_uart_tx();
