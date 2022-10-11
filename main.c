/* Universidad del Valle de Guatemala
 IE2023 Programación de Microcontroladores
 Autor: Luis Pablo Carranza
 Compilador: PIC-AS (v2.4), MPLAB X IDE (v6.00)
 Proyecto: Proyecto de laboratorio 1
 Hardware PIC16F887
 Creado: 23/08/22
 Última Modificación: 04/09/22*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT // Oscillator Selection bits (INTOSC 
//oscillator without clock out)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and 
//can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
//pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
//protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
//protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/
//External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-
//Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin 
//has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
//(Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
//(Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000

void setup(void);
void setup_portb(void);

void __interrupt() isr (void){
    if (RBIF == 1){
    if (PORTBbits.RB0 == 0)
    {
        PORTC++;
        INTCONbits.RBIF = 0;
    }
    else if (PORTBbits.RB1 == 0){
        PORTC--;
        INTCONbits.RBIF = 0;
    }
    }
}
void main(void) {
    setup();
    setup_portb();
    while (1){
    }
}
void setup(void){
    ANSELH = 0;
    TRISB = 0b00000111;
    TRISC = 0;    
    PORTB = 0;    
    PORTC = 0;
}
void setup_portb(void){
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCB = 0b00000111;
    WPUB = 0b00000111;
    OPTION_REGbits.nRBPU = 0;
}

