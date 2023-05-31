/* 
 * File:   main.c
 * Author: Mieko
 *
 * Created on November 3, 2022, 9:13 PM
 */

#define FCY 8000000

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p30fxxxx.h>
#include <libpic30.h>
#include "uart_driver.h"


// DSPIC30F4013 Configuration Bit Settings
// 'C' source line config statements
// FOSC
#pragma config FOSFPR = XT_PLL4         // Oscillator (XT w/PLL 4x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)
// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)
// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)
// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)
// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

// _FOSC(CSW_FSCM_OFF & XT_PLL4); // instrukcioni takt je isti kao i kristal
// _FWDT(WDT_OFF);

unsigned int brojac_ms, stoperica, ms, sekund;
unsigned int brojac_ms2, stoperica2, ms2, sekund2;
int cont;
float dist;

char dists[6]="";

/* Funckija stopira program na odredjeno vreme */ 
void delay(uint16_t delay_time);

void Delay_us (int vreme)//funkcija za kasnjenje u milisekundama
	{
		stoperica = 0;
		while(stoperica < vreme);
	}

void Delay_ms (int vreme2)//funkcija za kasnjenje u milisekundama
	{
		stoperica2 = 0;
		while(stoperica2 < vreme2);
	}


void __attribute__ ((__interrupt__,no_auto_psv)) _T1Interrupt(void) // svakih 1ms
{

		TMR1 =0;
     ms=1;//fleg za milisekundu ili prekid;potrebno ga je samo resetovati u funkciji

	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms

    if (brojac_ms==10000)//sekunda
        {
          brojac_ms=0;
          sekund=1;//fleg za sekundu
		 } 
	IFS0bits.T1IF = 0; 
       
}

void __attribute__ ((__interrupt__,no_auto_psv)) _T2Interrupt(void) // svakih 1ms
{

		TMR2 =0;
     ms2=1;//fleg za milisekundu ili prekid;potrebno ga je samo resetovati u funkciji

	brojac_ms2++;//brojac milisekundi
    stoperica2++;//brojac za funkciju Delay_ms

    if (brojac_ms2==10000)//sek
        {
          brojac_ms2=0;
          sekund2=1;//fleg za sekundu
		 } 
	IFS0bits.T2IF = 0; 
       
}

/*
 * Main funkcija
 */
int main(int argc, char** argv)
{
    // uint8_t len = 0;
    //uint8_t str[MAX_BUFFER_SIZE];
    
    TRISBbits.TRISB0=0; //trig na izlaz
    TRISBbits.TRISB1=1; //echo na ulaz 

    //XLCDInit();
    Init_T1();
    Init_T2();
      
    uartInit();
    uartWriteString("START\r\n");
    
    while(1)
    {
        
        //while(uartAvailable() == 0u);
        delay(10);
        /*
        len = uartReadString(str);
        
        if (len != 5)
        {
            uartWriteString("Uneli ste pogresan broj karaktera.\r\n");
        } else if ( (str[0] != 'D') || (str[1] != 'O') || (str[2] != 'R') || (str[3] != 'O') || (str[4] != 'S')  )
        {
            uartWriteString("Pogresan unos.\r\n");
        } else
        {
            uartWriteString("Jel te koleginice, sta kolega hoce od vas?\r\n");
        }
        */
       
        uartWriteString("timer start2 \r\n");
        LATBbits.LATB0=1;   // ukljucimo trig signal
        
        uartWriteString("ukljucen trig \r\n");
        //Delay_us(vrm1);        // duzina signala 10 
        __delay_us(10);        // duzina signala 10 us
        
        LATBbits.LATB0=0;   // ugasimo trig signal
        
        uartWriteString("iskljucen trig \r\n");
        while (!PORTBbits.RB1); //kad je echo na 1
        
        uartWriteString("timer start4 \r\n");
        T1CONbits.TON=1;
        
        uartWriteString("timer start \r\n");
        while (PORTBbits.RB1 || IFS0bits.T1IF==1); //kad je echo na 0
        
        T1CONbits.TON=0;
        uartWriteString("timer stop \r\n");
        
        
        TMR1=0;
        T1CONbits.TON=0;
        T1CONbits.TCKPS=0b10;
        if(PORTBbits.RB1==0)        //
            {
            cont=TMR1;
            dist=(float)cont*12.8/58; // cm
            uartWriteString("\r\n");
            uartWriteString((char)dist);
          
        }
        if(IFS0bits.T1IF==1)
            {IFS0bits.T1IF==1;}
    }
    
   
    
    return (EXIT_SUCCESS);
}

void delay (uint16_t delay_time)
{
    uint16_t i, j;
 
    for (i = 0; i < delay_time; i++)
        for (j = 0; j < 1000; j++);
}
