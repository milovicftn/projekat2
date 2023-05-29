/* 
 * File:   main.c
 * Author: MarkoSSD
 *
 * Created on 29. maj 2023., 04.05
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p30fxxxx.h>
#include "uart_driver.h"

_FOSC(CSW_FSCM_OFF & XT_PLL4); // instrukcioni takt je isti kao i kristal
_FWDT(WDT_OFF);

/* Funckija stopira program na odredjeno vreme */ 
void delay(uint16_t delay_time);

/*
 * Main funkcija
 *///
int main(int argc, char** argv)
{
    uint8_t len = 0;
    uint8_t len1 = 0;
    uint8_t str[MAX_BUFFER_SIZE];
   
    
    uartInit();
    uartWriteString("START\r\n");
    
    while(1)
    {
        while(uartAvailable() == 0u);
        delay(10);
        
        len = uartReadString(str);
        
        //if(len[] != len1[]){ 
            if (len != 5)// broj karaktera
            {
            uartWriteString("Uneli ste pogresan broj karaktera.\r\n");
            //len1=len;
            } else if ( (str[0] != 'D')   || (str[1] != 'O') || (str[2] != 'R') || (str[3] != 'O') || (str[4] != 'S'))  
            
            {
            uartWriteString("Pogresan unos.\r\n");
           // len1=len;
            } else
            {
            uartWriteString("Jel te koleginice, sta kolega hoce od vas?\r\n");
            }
        //} //else uartWriteString("Upisi str.\r\n"); 
        
       
    }

    return (EXIT_SUCCESS);
}

void delay (uint16_t delay_time)
{
    uint16_t i, j;
 
    for (i = 0; i < delay_time; i++)
        for (j = 0; j < 1000; j++);
}
