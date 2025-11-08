#include "uart.h"


volatile uint8_t RX_BUF[RX_BUF_LEN];
volatile uint16_t rx_head;
volatile uint16_t rx_tail;

volatile uint8_t TX_BUF[TX_BUF_LEN];
volatile uint16_t tx_head;
volatile uint16_t tx_tail;


void usart_initialize(uint16_t baud_rate)
{
    UBRR3H = (uint8_t)(baud_rate >> 8);
    UBRR3L = (uint8_t)(baud_rate & 0xFF);

    UCSR3B = TRANSMITTER | RECEIVER;

    UCSR3B |= (1 << RXCIE3); // enable la interrupts pe RX

    UCSR3C |= (1<<UPM31); // even parity
    UCSR3C |= (1<<USBS3); //2 stop bits
    UCSR3C |= ((1<<UCSZ31) | (1<<UCSZ30)); // 011 character size = 8bits

    TXD_OUT();
    RXD_IN();
}

uint16_t get_next_idx(uint16_t current_idx, uint16_t buflen)
{
    return (current_idx + 1) % buflen;
}



// void usart_transmit(uint8_t data)
// {
//     while (!(UCSR3A & (1 << UDRE3)))
//     {
//         // Se așteaptă până când buffer-ul de transmisie e gol
//     }
//     UDR3 = data;
// }

// uint8_t usart_receive(void)
// {
//     while (!(UCSR3A & (1 << RXC3)))
//     {
//         // Se așteaptă recepționarea unui caracter
//     }
//     return UDR3;
// }

void put_uart_rx(uint8_t data)
{
    // apelata in main deci trb oprite intreruperile
    uint16_t rx_head_tmp = rx_head;
    uint16_t rx_tail_tmp = rx_tail;

    uint16_t next_rx_tail = get_next_idx(rx_tail_tmp, RX_BUF_LEN);

    if (data != '\0')
    {
        if (next_rx_tail != rx_head_tmp)
        {
            RX_BUF[rx_tail_tmp] = data;
        }
    }

    rx_tail = next_rx_tail;
} // apelata de intr

uint8_t get_uart_rx()
{
    __disable_interrupt();

    uint16_t rx_head_tmp = rx_head;
    uint16_t rx_tail_tmp = rx_tail;
    uint16_t next_rx_head = get_next_idx(rx_head_tmp,RX_BUF_LEN);
    __enable_interrupt();

    if (rx_head_tmp == rx_tail_tmp)
    {
        return 0;
    } // coada goala

    __enable_interrupt();
    uint8_t return_data = RX_BUF[rx_head];
    next_rx_head = get_next_idx(rx_head,RX_BUF_LEN);
    rx_head = next_rx_head;

    __enable_interrupt();

    return return_data;

} // apelata de user






uint8_t get_uart_tx()
{
    uint16_t tx_head_tmp = tx_head;
    uint16_t tx_tail_tmp = tx_tail;

    if (tx_head_tmp == tx_tail_tmp)
    {
        // coada goala, se iese
        UCSR3B &= ~(1<<UDRE3); // DISABLE la intr pe UDRE, coada e goala, nu vreau sa se declanseze in gol
        return 0; // caracter invalid, trb verificat in intr
    }

    uint8_t elem_to_return = TX_BUF[tx_head_tmp];
    uint16_t next_tx_head = get_next_idx(tx_head_tmp,TX_BUF_LEN);
    tx_head = next_tx_head;

    return elem_to_return;

} // apelata de intr




void put_uart_tx(uint8_t data)
{
    __disable_interrupt();
    uint16_t tx_head_tmp = tx_head;
    uint16_t tx_tail_tmp = tx_tail;
    uint16_t next_tx_tail = get_next_idx(tx_tail_tmp,TX_BUF_LEN);
    __enable_interrupt();

    if(next_tx_tail == tx_head_tmp)
    { // coada plina, se iese
        return;
    }

    __disable_interrupt();
    TX_BUF[tx_tail] = data;
    next_tx_tail = get_next_idx(tx_tail_tmp,TX_BUF_LEN);
    tx_tail = next_tx_tail;

    UCSR3B |= (1<<UDRE3); // enable la intreruperea pe UDRE
    __enable_interrupt();

} // apelata de user

