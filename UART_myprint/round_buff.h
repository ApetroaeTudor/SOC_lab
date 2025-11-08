#ifndef __ROUND_BUFF__
#define __ROUND_BUFF__
// Biblioteca standard
#include <stdint.h>
#include <stddef.h>
#include <inavr.h>
#include <ioavr.h>
// Dimensiunea bufferului circular
#define BUFFER_SIZE 16
// Structura utilizata pentru buffer circular
struct round_buff
{
    uint8_t buffer[BUFFER_SIZE]; // Array-ul de date
    uint8_t head;                // Indexul de citire
    uint8_t tail;                // Indexul de scriere
};
// Tip definit pentru buffer circular
typedef struct round_buff round_buff_s;
// Buffer folosit pentru transmisie
extern round_buff_s tx_buffer;
// Buffer folosit pentru recep?ie
extern round_buff_s rx_buffer;
// Func?ie folosita pentru adaugarea unui caracter �n buffer
int16_t push(round_buff_s *in_buffer, uint8_t data);
// Func?ie folosita pentru extragerea unui caracter din buffer
uint8_t pop(round_buff_s *in_buffer);
// Func?ie folosita pentru adaugarea unui ?ir de caractere �n buffer
int16_t push_vec(round_buff_s *in_buffer, uint8_t data[], int16_t length);
// Func?ie folosita pentru verificarea daca bufferul este gol
int16_t is_empty(round_buff_s *in_buffer);
// Func?ie folosita pentru verificarea daca bufferul este plin
int16_t is_full(round_buff_s *in_buffer);
#endif
