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
#include "Tajmeri.h"


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


//C:\Mikroe\M\aaa.exe -w -${Device} -q -f"D:\PRIM\projekat2.X\dist\default\production\projekat2.X.production.hex"

 //_FOSC(CSW_FSCM_OFF & XT_PLL4); // instrukcioni takt je isti kao i kristal
 //_FWDT(WDT_OFF);

unsigned int stoperica;
unsigned int stoperica2;
int cont, i;
float dist;

char dists[6]="";

/* Funckija stopira program na odredjeno vreme */ 


void Delay_us (int vreme)//funkcija za kasnjenje u 10mikro
{
	stoperica = 0;
	while(stoperica < vreme);
}

void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
{
	stoperica2 = 0;
	while(stoperica2 < vreme);
}

void __attribute__ ((__interrupt__,no_auto_psv)) _T1Interrupt(void) // svakih 10us
{
	TMR1 =0;
    stoperica++;//brojac za funkciju Delay_ms
	IFS0bits.T1IF = 0;
}

void __attribute__ ((__interrupt__,no_auto_psv)) _T2Interrupt(void) // svakih 1ms
{
	TMR2 =0;
    stoperica2++;//brojac za funkciju Delay_ms
	IFS0bits.T2IF = 0;    
}

/*
 * Main funkcija
 */

void delay(uint16_t delay_time);

int main(int argc, char** argv)
{      
    uint8_t len = 0;
    uint8_t str[MAX_BUFFER_SIZE];
    
    //setovanje ulaza i izlaza
    //TRISBbits.TRISB0=0; //trig1 na izlaz
    //TRISBbits.TRISB1=1; //echo1 na ulaz 
    
    //inicijalizacije
    Init_T1();
    Init_T2();
    uartInit();
    uart2Init();
    
    //T1CONbits.TON=1;//obrisi

    Delay_ms(30); 
    uart2WriteString("START  2 \r \n");
    //RS232_putst2("START");
    //WriteUART2(13);
    
    //RS232_putst("START");
   /*
    while(1)
    {
        while(uartAvailable() == 0);
        //Delay_us(20);
       
        len = uartReadString(str);
        
        if (len != 5)
        {
            uart2WriteString("Uneli ste pogresan broj karaktera.\r\n");
        } else if ( (str[0] != 'D') || (str[1] != 'O') || (str[2] != 'R') || (str[3] != 'O') || (str[4] != 'S')  )
        {
            uart2WriteString("Pogresan unos.\r\n");
        } else
        {
            uart2WriteString("Jel te koleginice, sta kolega hoce od vas?\r\n");
        }
        
        /*
        //trig1
        LATBbits.LATB0=1;   // ukljucimo trig signal
        Delay_us(5);        // duzina signala 50us (ne moze manje)
        LATBbits.LATB0=0;   // ugasimo trig signal
        
        while (LATBbits.LATB1 == 0);//cekaj echo
        
        //T1CONbits.TON=1;
        TMR1=0;
        while (PORTBbits.RB1==1 || IFS0bits.T1IF==1); //dok je echo 1
        //T1CONbits.TON=0;
        cont=TMR1;
        dist=(float)cont*340/2; // cm (stavi *0.178573 )
        WriteUART2dec2string((int)dist);
        
        //T1CONbits.TON=0;
        //T1CONbits.TCKPS=0b10;
        
        /*if(PORTBbits.RB1==0)        //
            {
            cont=TMR1;
            dist=(float)cont*0.178573; // cm (stavi *0.178573 )
            uartWriteString("\r\n");
            uartWriteString((char)dist);
          
        }
        if(IFS0bits.T1IF==1)
            {IFS0bits.T1IF==1;}
        */
        
        
        //Delay_ms(200);
    
            
     
      while(1)
    {
        while(uartAvailable() == 0u);
        delay(10);
        
        len = uartReadString(str);
        
        if (len != 5)
        {
            uart2WriteString("Uneli ste pogresan broj karaktera.\r\n");
        } else if ( (str[0] != 'D') || (str[1] != 'O') || (str[2] != 'R') || (str[3] != 'O') || (str[4] != 'S')  )
        {
            uart2WriteString("Pogresan unos.\r\n");
        } else
        {
            uart2WriteString("Jel te koleginice, sta kolega hoce od vas?\r\n");
        }
    }      
    
    
   
    
    return (EXIT_SUCCESS);
}

void delay (uint16_t delay_time)
{
    uint16_t i, j;
 
    for (i = 0; i < delay_time; i++)
        for (j = 0; j < 1000; j++);
}
