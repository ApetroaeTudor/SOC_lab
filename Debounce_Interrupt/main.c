#include <ioavr.h>
#include <intrinsics.h>
#include <stdint.h>

// btn T4
#define BUTTON_PIN_REG PINE
#define BUTTON_DDR DDRE
#define BUTTON_PORT PORTE
#define BUTTON_PIN 6

// LED A pe PA5
#define LED_A_DDR DDRA
#define LED_A_PORT PORTA
#define LED_A_PIN 5

// LED B pe PA6
#define LED_B_DDR DDRA
#define LED_B_PORT PORTA
#define LED_B_PIN 6

volatile uint8_t btn_state = 1;

// Variabila pentru stocarea timpului scurs, similar cu millis()
volatile unsigned long system_millis = 0;
// Variabile pentru starea debounce

// Starea anterioară a butonului (1 = neapăsat)
uint8_t last_button_state = 1;

// Starea stabilă a butonului
uint8_t debounced_button_state = 1;

// Timpul de la ultimul debounce
unsigned long last_debounce_time = 0;

// Timpul de așteptare debounce (în ms)
const unsigned int debounce_delay = 50;

// ISR pentru TIMER0 (ceasul sistemului)
#pragma vector = TIMER0_COMPA_vect
__interrupt void TIMER0_ISR(void)
{
    btn_state = (PINE & (1 << BUTTON_PIN));
    // ENABLE BTN INERRUPTS
    EIMSK |= (1 << INT6);

    // DISABLE TIMER INTERRUPTS
    TIMSK0 &= ~(1 << OCIE0A);
}

// Funcția de inițializare timer
void init_millis_timer(void)
{
    /*
     * Configurare Timer0 mod CTC pentru a genera o întrerupere la 1 ms
     * F_CPU = 16 MHz. Prescaler = 64. F_timer = 16 MHz / 64 = 250 kHz.
     * Pentru 1 ms (1 kHz), OCR0A = (250 kHz / 1 kHz) - 1 = 249.
     */
    TCCR0A |= (1 << WGM01);              // Mod CTC
    TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249;
}

/*
 * Funcția configurează butonul (PE6) ca intrare cu pull-up și LED-urile
 * (PA5, PA6) ca ieșiri, pornind cu ambele stinse.
 */
void init_gpio(void)
{
    // Configurare Buton (PE6) ca intrare cu pull-up
    BUTTON_DDR &= ~(1 << BUTTON_PIN);
    BUTTON_PORT |= (1 << BUTTON_PIN);
    // Configurare LED-uri (PA5, PA6) ca ieșiri
    LED_A_DDR |= (1 << LED_A_PIN);
    LED_B_DDR |= (1 << LED_B_PIN);

    // Se opresc LED-urile inițial
    LED_A_PORT &= ~(1 << LED_A_PIN);
    LED_B_PORT &= ~(1 << LED_B_PIN);
}

void init_btn_interrupt(void)
{
    // PE6 - INT 6
    EICRB |= (1 << ISC61); // neg edge la apasarea butonului (pullup resistor e activ)
    EIMSK |= (1 << INT6);  // activare external interrupt bit 6 (BTN t4)
}

#pragma vector = INT6_vect
__interrupt void INT6_ISR(void)
{
    // intreruperea pe apasarea de buton
    //  aici btn state se stie ca e 0.

    // DISABLE BTN INTERRUPT
    EIMSK &= ~(1 << INT6);

    // ENABLE TIMER INTERRUPT
    TIMSK0 |= (1 << OCIE0A); // Activează întreruperea pe Compare Match A
}

int main(void)
{
    init_gpio();
    init_millis_timer();
    init_btn_interrupt();
    __enable_interrupt();
    while (1)
    {
        if (btn_state == 0)
        {
            LED_A_PORT ^= (1 << LED_A_PIN);
            btn_state = 1;
        }
        
    }
}
