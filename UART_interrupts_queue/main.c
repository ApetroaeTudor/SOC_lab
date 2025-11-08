#include "uart.h"

#pragma vector = USART3_UDRE_vect
__interrupt void USART3_UDRE_ISR(void)
{
    // se ia data din coada de tx, se verifica daca e valida si se pune in UDR3
    uint8_t data_to_send = get_uart_tx();
    if(data_to_send!=0)
    {
        UDR3= data_to_send;
    }
}

#pragma vector = USART3_RX_vect
__interrupt void USART3_RX_ISR(void)
{
    // se ia din UDR, se verifica daca e diferit de /0, si se pune in coada de rx
    uint8_t data_read = UDR3;
    if(data_read!=0)
    {
        put_uart_rx(data_read);
    }
}

void main(void)
{
    uint8_t aux;
    // Se inițializează modulul USART cu Baud Rate-ul predefinit
    usart_initialize(BAUD_RATE);

    // Buclă infinită pentru blocarea procesorului și crearea efectului de ecou
    while (1)
    {
        aux = get_uart_rx();
        if(aux!=0)
        {
            put_uart_tx(aux);
            aux = 0;
        }
    }
}
