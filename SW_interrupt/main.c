// Includes

#include <ioavr.h>
#include <intrinsics.h>

/*
 * Funcția configurează întreruperea externă INT0 pe frontul pozitiv.
 * Aceasta setează toți pinii PORTD ca ieșire și aprinde inițial toate
 * LED-urile, după care activează întreruperile globale.
 */
// ISR pentru INT0
#pragma vector = INT0_vect
__interrupt void INT0_ISR(void)
{
    static unsigned char test = 0;
    test += 1;
}

void init_interrupt_INT0(void)
{
    // Se setează toți pinii PORTD ca ieșire (LED-uri, test)
    DDRD = 0xFF; // 0 e intrare, 1 e iesire
    // cand scriu in PORTD pe front pozitiv se declanseaza intr si se incr test

    // Inițial, toate LED-urile sunt aprinse (logica inversă)
    PORTD = 0xFF;
    
    // Se configurează INT0 pe front pozitiv
    EICRA |= (1 << ISC01) | (1 << ISC00);
    
    // Se activează întreruperea INT0
    EIMSK |= (1 << INT0);
    
    // Se activează întreruperile globale
    __enable_interrupt();
}

int main(void)
{
    init_interrupt_INT0();
    // Buclă infinită pentru generarea unei întreruperi software
    while (1)
    {
        PORTD = ~PORTD; // trec pe 0 toate.
        __delay_cycles(16000000);
    }
}
