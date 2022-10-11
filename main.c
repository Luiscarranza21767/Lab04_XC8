/* Universidad del Valle de Guatemala
 IE2023 Programación de Microcontroladores
 Autor: Luis Pablo Carranza
 Compilador: XC8, MPLAB X IDE (v6.00)
 Proyecto: Laboratorio 4
 Hardware PIC16F887
 Creado: 10/10/22
 Última Modificación: 10/10/22*/

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
void setup_ADC(void);
int DISP1 = 0;
int DISP2 = 0;
uint8_t display[16] = {
    0b00111111, 
    0b00000110, 
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111,  
    0b01110111,
    0b01111100,
    0b00111001,
    0b01011110,
    0b01111001,
    0b01110001};

void __interrupt() isr (void){
    if (RBIF == 1){
    if (PORTBbits.RB6 == 0)
    {
        __delay_ms(30);
        if (PORTBbits.RB6 == 1){
            PORTC++;
            INTCONbits.RBIF = 0;
        }
    }
    else if (PORTBbits.RB7 == 0){
        __delay_ms(30);
        if (PORTBbits.RB7 == 1){
            PORTC--;
            INTCONbits.RBIF = 0;}
    }
    }
}
void main(void) {
    setup();
    setup_portb();
    setup_ADC();
    PORTD = display[0];
    while (1){
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO == 1);
        ADIF = 0;
        DISP1 = (ADRESH%16);
        DISP2 = (ADRESH/16);           
        if (PORTBbits.RB0 == 1){
            PORTD = display[DISP1];
            PORTBbits.RB0 = 0;
            PORTBbits.RB1 = 1;
            }
        else if(PORTBbits.RB1 == 1){
            PORTD = display[DISP2];
            PORTBbits.RB0 = 1;
            PORTBbits.RB1 = 0;
            }
        __delay_ms(5); 
    }
}
void setup(void){
    ANSELH = 0;
    TRISB = 0b11100000;
    TRISC = 0; 
    TRISD = 0;
    PORTC = 0;
    PORTB = 1;
    PORTD = 0;
}
void setup_portb(void){
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCB = 0b11100000;
    WPUB = 0b11100000;
    OPTION_REGbits.nRBPU = 0;
    
}
void setup_ADC(void){
    PORTAbits.RA0 = 0;
    TRISAbits.TRISA0 = 1;
    ANSELbits.ANS0 = 1;
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;   // Fosc/8
    
    ADCON1bits.VCFG1 = 0;   // Ref VSS
    ADCON1bits.VCFG0 = 0;   // Ref VDD
    
    ADCON1bits.ADFM = 0;    // Justificado a la izquierda
    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        // Selección del canal AN0
    
    ADCON0bits.ADON = 1;        // Habilitar el convertidor ADC
    __delay_us(100);
    
}

