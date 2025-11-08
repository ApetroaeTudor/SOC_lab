#ifndef __USART__
#define __USART__

#include <inavr.h>
#include <ioavr.h>
#include "round_buff.h"

#define F_OSC 16000000UL
#define BAUD 9600
#define BAUD_RATE (F_OSC / 16 / BAUD - 1)


// Func?ie folosita la ini?ializarea modulului USART
void USART_initialize(uint16_t baud_rate);

// Func?ie folosita la ransmiterea unui singur caracter
uint16_t USART_transmit_char(uint8_t c);

// Func?ie folosita la transmiterea a mai multor caractere
uint16_t USART_transmit_string(uint8_t *s, int16_t length);

// Func?ie folosita pentru recep?ia unui singur caracter
uint16_t USART_receive_char(uint8_t *c);

// Func?ie folosita pentru recep?ia a mai multor caractere
uint16_t USART_receive_string(uint8_t *c, uint16_t length);

// Func?ie folosita la �ntreruperea pentru DATA REGISTER EMPTY
#pragma vector = USART1_TX_vect
__interrupt void USART1_TX_ISR(void);

// Func?ie folosita la �ntreruperea pentru RECEIVE COMPLETE
#pragma vector = USART1_RX_vect
__interrupt void USART1_RX_ISR(void);
#endif
