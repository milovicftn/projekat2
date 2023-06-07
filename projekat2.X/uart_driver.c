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

void uart2Init(void)
{
    U2BRG=0x0033;//ovim odredjujemo baudrate

    //U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

    IEC1bits.U2RXIE=1;//omogucavamo rx1 interupt

    U2STA&=0xfffc;

    U2MODEbits.UARTEN=1;//ukljucujemo ovaj modul

    U2STAbits.UTXEN=1;//ukljucujemo predaju
}

void WriteUART1(unsigned int data)
{
	 while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART2(unsigned int data)
{
	while(!U2STAbits.TRMT);

    if(U2MODEbits.PDSEL == 3)
        U2TXREG = data;
    else
        U2TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned int data)
{
	unsigned char temp;
    temp=data/10000;
	WriteUART1(temp+'0');
	data=data-temp*10000;
	temp=data/1000;
	WriteUART1(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART1(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART1(temp+'0');
	data=data-temp*10;
	WriteUART1(data+'0');
}

void WriteUART2dec2string(unsigned int data)
{
	unsigned char temp;
    temp=data/10000;
	WriteUART2(temp+'0');
	data=data-temp*10000;
	temp=data/1000;
	WriteUART2(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART2(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART2(temp+'0');
	data=data-temp*10;
	WriteUART2(data+'0');
}

void RS232_putst(register const char*str)
{
    while(*str)
    {
        WriteUART1(*str++);
       // WriteUART1(10);
    }
}

void RS232_putst2(const char *s)
{
    while(*s)
    {
        WriteUART2(*s++);
        //WriteUART2(10);
    }
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

void uartWriteChar2(uint8_t p_char)
{
	while(!U2STAbits.TRMT);

    if(U2MODEbits.PDSEL == 3)
        U2TXREG = p_char;
    else
        U2TXREG = p_char & 0xFF;
}

void uartWriteString(uint8_t *p_str)
{
	while ((*p_str) != 0)
    {
        uartWriteChar(*p_str);
        p_str++;
    }

}

void uart2WriteString(uint8_t *p_str)
{
	while ((*p_str) != 0)
    {
        uartWriteChar2(*p_str);
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

void __attribute__((__interrupt__,no_auto_psv)) _U2RXInterrupt(void) 
{
    //IFS0bits.U1RXIF = 0;
    IFS1bits.U2RXIF = 0;
    
    //rx_buffer[buffer_last++]=U1RXREG;
    rx_buffer[buffer_last++]=U2RXREG;
    buffer_last &= MAX_BUFFER_SIZE - 1;
    
    if (buffer_size < MAX_BUFFER_SIZE)
    {
        buffer_size++;
    }
} 
