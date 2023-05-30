/* 
 * File:   uart_driver.h
 * Author: Mieko
 *
 * Created on November 3, 2022, 10:33 PM
 */

#include "uart_driver.h"

/* Prijemni FIFO bafer */
uint8_t rx_buffer[MAX_BUFFER_SIZE];
/* Pozicija sa koje ce se procitati sledeci karakter */
volatile uint8_t buffer_first = 0;
/* Pozicija na koju ce se upisati sledeci karakter */
volatile uint8_t buffer_last  = 0;
/* Trenutni broj karaktera u baferu */
volatile uint8_t buffer_size  = 0;

void uartInit(void)
{
    U1BRG=0x0033;//ovim odredjujemo baudrate

    U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

    IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt

    U1STA&=0xfffc;

    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

    U1STAbits.UTXEN=1;//ukljucujemo predaju
}

uint8_t uartAvailable()
{
    return buffer_size;
}

uint8_t uartReadChar()
{
    uint8_t char_to_return;
    
    if (buffer_size == 0)
    { 
        return -1;
    }
    
    char_to_return = rx_buffer[buffer_first++];
    buffer_first &= MAX_BUFFER_SIZE - 1;
    
    buffer_size--;
    
    return char_to_return;
}

uint8_t uartReadString(uint8_t *str_to_read)
{
    uint8_t length = 0u;
    
    while (buffer_size > 0)
    {
        str_to_read[length++] = uartReadChar();
    }
    
    return length;
}

void uartWriteChar(uint8_t p_char)
{
	while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = p_char;
    else
        U1TXREG = p_char & 0xFF;
}

void uartWriteString(uint8_t *p_str)
{
	while ((*p_str) != 0)
    {
        uartWriteChar(*p_str);
        p_str++;
    }

}

void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    
    rx_buffer[buffer_last++]=U1RXREG;
    buffer_last &= MAX_BUFFER_SIZE - 1;
    
    if (buffer_size < MAX_BUFFER_SIZE)
    {
        buffer_size++;
    }
} 