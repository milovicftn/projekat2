#include <p30fxxxx.h>
#include "tajmeri.h"

#define TMR2_period 8000 /*  Fosc = 8MHz,
					          1/Fosc = 0.1us !!!, 0.1us * 10000 = 1ms 
					          1/8000 = 0.125us !!!, 0.125us * 8000 = 1ms  */
#define TMR1_period_us 80 // 10 uS 

//#define TMR2_period 8 /*  Fosc = 10MHz,
					         // 1/Fosc = 0.1us !!!, 0.1us * 1000 = 0.1ms  */

void Init_T1(void)//tajmer za digitalni senzor 1
{
	TMR1 = 0;
	PR1 = TMR1_period_us;
	
	T1CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T2IP = 3 // T2 interrupt pririty (0-7)
//	SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T1IF = 0; // clear interrupt flag
	IEC0bits.T1IE = 1; // enable interrupt

	T1CONbits.TON = 0; // T1 off, upali pri merenju echo pina
}

void Init_T2(void)//tajmer za delay ms
{
	TMR2 = 0;
	PR2 = TMR2_period;
	
	T2CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T2IP = 3 // T2 interrupt pririty (0-7)
	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T2IF = 0; // clear interrupt flag
	IEC0bits.T2IE = 1; // enable interrupt

	T2CONbits.TON = 1; // T2 off, upali pri merenju echo pina 
}

void Init_T3(void)//tajmer za digitalni senzor 2
{
	TMR3 = 0;
	PR3 = TMR1_period_us;
	
	T3CONbits.TCS = 0; // 0 = Internal clock (FOSC/4)
	//IPC1bits.T2IP = 3 // T2 interrupt pririty (0-7)
	//SRbits.IPL = 3; // CPU interrupt priority is 3(11)
	IFS0bits.T3IF = 0; // clear interrupt flag
	IEC0bits.T3IE = 1; // enable interrupt

	T3CONbits.TON = 0; // T3 off, upali pri merenju echo pina 
}


//---------------------------------------
