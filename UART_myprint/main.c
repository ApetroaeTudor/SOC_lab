#include "mylib.h"
#include "usart.h"

// trb crescut CSTACK size de ex la 0x40 (de la 0x20)

int16_t main(void)
{
    USART_initialize(BAUD_RATE);
    __enable_interrupt();

    // Se define?te variabila transmisa
    double a = 1.2;
    // Se define?te pasul de incrementare/decrementare
    double pas = 0.5;

    while (1)
    {
        // Se transmite valoarea variabilei prin USART
        my_print(DOUBLE, &a);

        // Se actualizeaza valoarea variabilei
        a += pas;

        // Daca se ating limitele, direc?ia de varia?ie se inverseaza
        if (a >= 50.0 || a <= 0.0)
            pas *= -1;

        // Se introduce o �nt�rziere pentru stabilirea ritmului de transmitere
        __delay_cycles(1600000);
    }
}
