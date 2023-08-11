/*
 * File:   LCD.c
 * Author: Johann
 *
 * Created on 5 de agosto de 2023, 10:23 PM
 */

#include <stdint.h>
#include <xc.h>
#include "LCD.h"
#define _XTAL_FREQ  8000000

uint8_t CENTENA(unsigned char c){    //se separa el bit mas significativo   
    c = (uint8_t)((c*1.9607)/100);
    return c;
}
uint8_t DECENA(unsigned char c){         //se separa el bit medio significativo
    c = (uint8_t)((c*1.9607) - CENTENA(c)*100)/10;
    return c;
}

uint8_t UNIDAD(unsigned char c){         // se separa el bit menos significativo
    c = (uint8_t)((c*1.9607) - CENTENA(c)*100 - DECENA(c)*10);
    return c;
}


void IOC_INT(uint8_t a){        //configuracion de PULL UPS
                                //de los botones
    //Config. PULL UP
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB = a;
    IOCBbits.IOCB = a;         
    INTCONbits.RBIE = 1;    //interrupcion pull up 
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
}


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

void LCD_PORT(char a){
    PORT = a;
}
void LCD_COM(char a){
    RS_PIN = 0;
    LCD_PORT(a);
    E_PIN = 1;
    __delay_ms(5);
    E_PIN = 0;
}
void LCD_CLEAR(void){
    LCD_PORT(0);
    LCD_PORT(1); 
}
void LCD_XY(char x, char y){
    if(x == 1)
        LCD_COM(0x80 + y);
	else if(x == 2)
        LCD_COM(0xC0 + y);
}
void LCD_INIT(void){
    LCD_PORT(0x00);
	RS_PIN = 0;
	__delay_ms(25);
	///////////// Reset process from datasheet /////////
    LCD_COM(0x30);
  __delay_ms(5);
    LCD_COM(0x30);
      __delay_ms(15);
    LCD_COM(0x30);
    /////////////////////////////////////////////////////
    LCD_COM(0x38);    //function set
    LCD_COM(0x0C);    //display on,cursor off,blink off
    LCD_COM(0x01);    //clear display
    LCD_COM(0x06);    //entry mode, set increment
}
void LCD_CHAR(char a){
    RS_PIN = 1;             // => RS = 1
    LCD_PORT(a);    //Data transfer
    E_PIN  = 1;             // => E = 1
   __delay_ms(4);
    E_PIN  = 0;
}
void LCD_STRING(char *a){
    int i;
	for(i=0;a[i]!='\0';i++)
	 LCD_CHAR(a[i]);
}
