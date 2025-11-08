#include "round_buff.h"

int16_t push(round_buff_s *inBuffer, uint8_t data)
{
    uint8_t next_head = (inBuffer->head + 1) % BUFFER_SIZE;

    if (!is_full(inBuffer))
    {
        inBuffer->buffer[inBuffer->head] = data;
        inBuffer->head = next_head;
        // Returneaza 1 pentru succes
        return 1;
    }
    else
        // Returneaza 0 daca bufferul este plin
        return 0;
}
// Func?ie folosita pentru extragerea unui caracter din buffer
uint8_t pop(round_buff_s *inBuffer)
{
    if (!is_empty(inBuffer))
    {
        uint8_t data = inBuffer->buffer[inBuffer->tail];
        inBuffer->tail = (inBuffer->tail + 1) % BUFFER_SIZE;
        // Returneaza caracterul extras
        return data;
    }
    else
    {
        // Returneaza 0 daca bufferul este gol
        return 0;
    }
}


int16_t push_vec(round_buff_s *inBuffer, uint8_t data[], int16_t length)
{
    uint16_t i;
    /*
     * Se parcurge fiecare caracter din ?ir ?i se �ncearca adaugarea lui.
     * �n caz ca bufferul se umple, se opre?te inserarea.
     */
    for (i = 0; i < length; i++)
    {
        uint8_t tmp_data = data[i];
        uint8_t verif = push(inBuffer, tmp_data);
        if (verif == 0)
            break;
    }

    // Returneaza numarul de caractere adaugate cu succes
    return i;
}


// Func?ie folosita pentru verificarea daca bufferul este gol
int16_t is_empty(round_buff_s *inBuffer)
{
    return (inBuffer->head == inBuffer->tail);
}
// Func?ie folosita pentru verificarea daca bufferul este plin
int16_t is_full(round_buff_s *inBuffer)
{
    return ((inBuffer->head + 1) % BUFFER_SIZE) == inBuffer->tail;
}
