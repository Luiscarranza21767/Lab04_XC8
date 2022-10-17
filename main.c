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

//******************************************************************************
// Definición de funciones y variables
//******************************************************************************
void setup(void);
void setup_portb(void);
void setup_ADC(void);
void Comparador(void);

int DISP1 = 0;
int DISP2 = 0;

//******************************************************************************
// Arreglo para los valores del display
//******************************************************************************
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

//******************************************************************************
// Función de interrupciones
//******************************************************************************
void __interrupt() isr (void){
    if (RBIF == 1){             // Revisa si hay interrupción del puerto B
    if (PORTBbits.RB6 == 0)     // Si hay revisa si se presionó RB6
    {
        __delay_ms(30);         // Hace un delay para el antirrebote
        if (PORTBbits.RB6 == 1){// Revisa si ya se dejó de presionar
            PORTC++;            // Incrementa el puerto del contador
            INTCONbits.RBIF = 0;// Apaga la bandera de interrupción
        }
    }
    else if (PORTBbits.RB7 == 0){// Revisa si se presionó RB7
        __delay_ms(30);          // Hace un delay para antirrebotes
        if (PORTBbits.RB7 == 1){ // Revisa si ya se dejó de presionar el botón
            PORTC--;             // Decrementa el puerto del contador
            INTCONbits.RBIF = 0;}// Apaga la bandera de interrupciones
    }
    }
}
//******************************************************************************
// Función principal
//******************************************************************************
void main(void) {
    setup();            // Configuraciones inciales
    setup_portb();      // Configuración de interrupción del puerto B
    setup_ADC();        // Configuración del ADC
    PORTD = display[0]; // Asigna inicialmente un valor de 0 al puerto del disp
    while (1){
        ADCON0bits.GO = 1; // Inicia el ADC
        while (ADCON0bits.GO == 1); // Revisa si ya terminó la conversión ADC
        ADIF = 0;               // Apaga la bandera del ADC
        DISP1 = (ADRESH%16);    // Carga el residuo del resultado en 16 a DISP1
        DISP2 = (ADRESH/16);    // Carga el cociente del resultado dividido 16
        Comparador();           // Compara contador con el ADC
        if (PORTBbits.RB0 == 1){// Revisa cuál display está encendido
            PORTD = display[DISP1]; // Carga el valor del que está encendido 
            PORTBbits.RB0 = 0;      // Apaga el display que estaba encendido
            PORTBbits.RB1 = 1;      // Enciende el que corresponde
            }
        else if(PORTBbits.RB1 == 1){// Revisa cuál display está encendido
            PORTD = display[DISP2]; // Carga el valor correspondiente
            PORTBbits.RB0 = 1;      // Enciende el que toca
            PORTBbits.RB1 = 0;      // Apaga el que estaba encendido
            }
        __delay_ms(5);              // Espera 5ms y vuelve al ciclo
    }
}
//******************************************************************************
// Configuración de puertos
//******************************************************************************
void setup(void){
    ANSELH = 0;
    TRISB = 0b11000000; // Dos pines son entradas
    TRISC = 0; 
    TRISD = 0;
    PORTC = 0;
    PORTB = 1;
    PORTD = 0;
}
//******************************************************************************
// Configuración del puerto B
//******************************************************************************
void setup_portb(void){
    INTCONbits.GIE = 1;     // Habilita interrupciones globales
    INTCONbits.RBIE = 1;    // Habilita interrupción del puerto B
    INTCONbits.RBIF = 0;    // Apaga la bandera de interrupción del puerto B
    IOCB = 0b11000000;      // Habilita la interrupción en cambio
    WPUB = 0b11000000;      // Habilita el Weak Pull-Up en el puerto B
    OPTION_REGbits.nRBPU = 0;   // Deshabilita el bit de RBPU
    
}
//******************************************************************************
// Configuración del ADC
//******************************************************************************
void setup_ADC(void){
    PORTAbits.RA0 = 0;      // Inicia el bit 0 de PORTA en 0
    TRISAbits.TRISA0 = 1;   // RA0 es entrada
    ANSELbits.ANS0 = 1;     // RA0 es analógico
    
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
//******************************************************************************
// Función para comparar el contador con los displays
//******************************************************************************
void Comparador(void){
    if (ADRESH >= PORTC){   // Revisa si el resultado de la conversión es mayor
        PORTBbits.RB5 = 1;  // que el contador. Si si, enciende RB5
    }
    else {
        PORTBbits.RB5 = 0;  // Si no, lo apaga
    } 
}

