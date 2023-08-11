/*
 * File:   ADC.c
 * Author: Johann
 *
 * Created on 5 de agosto de 2023, 10:45 PM
 */

#include <stdint.h>
#include <xc.h>
#include "ADC.h"
#define _XTAL_FREQ  8000000



void OSCILLATOR(uint8_t f){         //Escogo que frecuencia quiero para el oscilador
    OSCCONbits.SCS = 1;
    
    switch(f){
        case(1):
            OSCCONbits.IRCF2 =1;        //8M Hz    
            OSCCONbits.IRCF1 =1;
            OSCCONbits.IRCF0 =1;
            break;
            
        case(2):
            OSCCONbits.IRCF2 =1;        //4M Hz    
            OSCCONbits.IRCF1 =1;
            OSCCONbits.IRCF0 =0;
            break;
            
        case(3):
            OSCCONbits.IRCF2 =1;        //2M Hz    
            OSCCONbits.IRCF1 =0;
            OSCCONbits.IRCF0 =1;
            break;
            
        case(4):
            OSCCONbits.IRCF2 =1;        //1M Hz    
            OSCCONbits.IRCF1 =0;
            OSCCONbits.IRCF0 =0;
            break;
            
        case(5):
            OSCCONbits.IRCF2 =0;        //500k Hz    
            OSCCONbits.IRCF1 =1;
            OSCCONbits.IRCF0 =1;
            break;
        
        case(6):
            OSCCONbits.IRCF2 =0;        //250k Hz    
            OSCCONbits.IRCF1 =1;
            OSCCONbits.IRCF0 =0;
            break;
        
        case(7):
            OSCCONbits.IRCF2 =0;        //125k Hz    
            OSCCONbits.IRCF1 =0;
            OSCCONbits.IRCF0 =1;
            break;
        
        case(8):
            OSCCONbits.IRCF2 =0;        //31k Hz    
            OSCCONbits.IRCF1 =0;
            OSCCONbits.IRCF0 =0;
            break;
        
        default:
            OSCCONbits.IRCF2 =1;        //4M Hz    
            OSCCONbits.IRCF1 =1;
            OSCCONbits.IRCF0 =0;
            break;
    }
    return;
}


void ADC_INIT(uint8_t c){               //Seleccion de canales
    switch(c){
        case 0:
            ADCON0bits.CHS3 = 0;    //canal 0
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 1:
            ADCON0bits.CHS3 = 0;    //canal 1
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 2:
            ADCON0bits.CHS3 = 0;    //canal 2
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 3:
            ADCON0bits.CHS3 = 0;    //canal 3
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 4:
            ADCON0bits.CHS3 = 0;    //canal 4
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 5:
            ADCON0bits.CHS3 = 0;    //canal 5
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 6:
            ADCON0bits.CHS3 = 0;    //canal 6
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 7:
            ADCON0bits.CHS3 = 0;    //canal 7
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 8:
            ADCON0bits.CHS3 = 1;    //canal 8
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 9:
            ADCON0bits.CHS3 = 1;    //canal 9
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 10:
            ADCON0bits.CHS3 = 1;    //canal 10
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 11:
            ADCON0bits.CHS3 = 1;    //canal 11
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 1;
            break;
            
        case 12:
            ADCON0bits.CHS3 = 1;    //canal 12
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
            
        case 13:
            ADCON0bits.CHS3 = 1;    //canal 13
            ADCON0bits.CHS2 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
            
        default:
            ADCON0bits.CHS3 = 0;    //canal 0
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;
            break;
    }
    
    ADCON1bits.VCFG0 = 0;   //VDD referencias internas
    ADCON1bits.VCFG1 = 0;   //VSS
    
    ADCON0bits.ADCS0 = 0;   //fosc/32
    ADCON0bits.ADCS1 = 1;

    ADCON1bits.ADFM =0;     //justificado izquierda
    
    ADCON0bits.ADON = 1;    //Habilito modulo ADC 
    __delay_us(50);
    ADCON0bits.GO_nDONE = 1;
    return;
}
unsigned char ADC_READ(void){                     //funcion que lee el potenciometro
    ADCON0bits.GO_nDONE = 1;
    __delay_ms(50);
    while( ADCON0bits.GO_nDONE == 1);
    return ADRESH;
            
}