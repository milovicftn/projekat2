/* 
 * File:   uart_driver.h
 * Author: Mieko
 *
 * Created on November 3, 2022, 10:33 PM
 */

#ifndef UART_DRIVER_H
#define	UART_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p30fxxxx.h>

#define MAX_BUFFER_SIZE 64u

/* Prijemni FIFO bafer */
extern uint8_t rx_buffer[MAX_BUFFER_SIZE];
/* Pozicija sa koje ce se procitati sledeci karakter */
extern volatile uint8_t buffer_first;
/* Pozicija na koju ce se upisati sledeci karakter */
extern volatile uint8_t buffer_last;
/* Trenutni broj karaktera u baferu */
extern volatile uint8_t buffer_size;    
    
/* Funckija inicijalizuje UART kanal */
void uartInit(void);
void uartInit2(void);

void WriteUART1(unsigned int data);
void WriteUART2(unsigned int data);
void WriteUART1dec2string(unsigned int data);
void RS232_putst(register const char*str);
void RS232_putst2(const char *s);

/* Funckija koja vraca trenutni broj karaktera u FIFO baferu 
   Ako je prazan vraca 0
 */ 
uint8_t uartAvailable();

/* Funckija cita jedan karakter iz FIFO bafera */
uint8_t uartReadChar();

/* Funckija cita sve karaktere u FIFO baferu */
uint8_t uartReadString(uint8_t *str_to_read);

/* Funckija upisuje jedan karakter u FIFO bafer */
void uartWriteChar(uint8_t p_char);
void uartWriteChar2(uint8_t p_char);

/* Funckija upisuje niz karaktera u FIFO bafer */
void uartWriteString(uint8_t *p_str);
void uart2WriteString(uint8_t *p_str);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_DRIVER_H */

