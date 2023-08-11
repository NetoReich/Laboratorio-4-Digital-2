/*
 * File:   Main_Español.c
 * Author: Johann
 *
 * Created on 5 de agosto de 2023, 10:23 PM
 */


#pragma config  FOSC    = INTRC_NOCLKOUT
#pragma config  WDTE    = OFF
#pragma config  PWRTE   = OFF
#pragma config  MCLRE   = OFF
#pragma config  CP      = OFF
#pragma config  CPD     = OFF
#pragma config  BOREN   = OFF
#pragma config  IESO    = OFF
#pragma config  FCMEN   = OFF
#pragma config  LVP     = OFF

#pragma config  BOR4V   = BOR40V
#pragma config  WRT     = OFF

#include <stdint.h>
#include <xc.h>
#include "LCD.h"
#include "I2CEspanol.h"

#define PUSH1 RB0
#define PUSH2 RB1
#define _XTAL_FREQ  8000000


uint8_t POT;
uint8_t unidad, decena,centena;

uint8_t seg,min,hora;
uint8_t dia,mes,year;

uint8_t select,parametro,p;

static char Time[]= "00/00/00";
static char Date[]="00/00/2000";

void setup(void);
__bit ANTI_REBOTE(void);

void RTC_PRINT(void);
void RTC_WRITE(unsigned I2C,unsigned d,char parametro);
unsigned short RTC_READ(unsigned I2C,unsigned d);

uint8_t PAM(uint8_t parametro,uint8_t rtc);
uint8_t CONFIGURAR_v2(uint8_t x, uint8_t y, uint8_t parametro,uint8_t rtc);
void Parpadeo(void);

uint8_t DECIMAL_BCD(uint8_t num);
uint8_t BCD_DECIMAL(uint8_t num);

void __interrupt() isr(void){
    if (INTCONbits.RBIF==1){               //Interrupciones de botones en PORTB con PULL UP
        if (PUSH1==0){
            if(ANTI_REBOTE())                   //RB0 incrementa select
                select++;
        } else if (PUSH2==0){               //RB1 incrementa parametro solo si 
            if(select != 0)                 //select no es 0
                if(ANTI_REBOTE()) 
                    parametro++;
        }
        if(select >5)                       //limites de ambas variables para que
            select = 0;                     //no salgan del rango querido
        
        if(select == 1 && parametro >23)       //horas
            parametro = 0;
        if(select == 2 && parametro >59) //minutos
            parametro = 0;
        if(select == 3 && parametro >12) //meses
            parametro = 1;
        if(select == 3 && parametro ==0) //meses
            parametro = 1;
        if(select == 4 && parametro >99) //años
            parametro = 0;
        
        INTCONbits.RBIF=0;
    }
}

void main(void) {
    setup();
    
    while(1){
        
        I2C_Master_Start();             //Lectura pic SLAVE1, potenciometro
        I2C_Master_Write(0x81);
        POT = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
       
        
        if(select !=0){
            if(select == 1){                        
                parametro = PAM(parametro,hora);
                hora = CONFIGURAR_v2(1,8,parametro,hora);
            }
            if(select == 2){
                parametro = PAM(parametro,min);
                min = CONFIGURAR_v2(1,11,parametro,min);
            }
            if(select == 3){
                parametro = PAM(parametro,mes);
                mes = CONFIGURAR_v2(2,9,parametro,mes);
            }
            if(select == 4){
                parametro = PAM(parametro,year);
                year = CONFIGURAR_v2(2,14,parametro,year);
            }
            if(select == 5){
                RTC_WRITE(0xD0,0x02,hora);      //funcion para reescribir el RTC
                RTC_WRITE(0xD0,0x01,min);
                RTC_WRITE(0xD0,0x05,mes);
                RTC_WRITE(0xD0,0x06,year);
                RTC_WRITE(0xD0,0x00,0x00);
                parametro = 0;
                select = 0;
            }
        }else {                     //si select = 0 entonces solo se lee RTC
        seg = RTC_READ(0xD0,0x00);
        min = RTC_READ(0xD0,0x01);
        hora = RTC_READ(0xD0,0x02);
        dia = RTC_READ(0xD0,0x04);
        mes = RTC_READ(0xD0,0x05);
        year = RTC_READ(0xD0,0x06); 
        }
        //LCD******************************************************************
        centena = CENTENA(POT);     //Aqui separamos el valor para poder
        decena = DECENA(POT);       //imprimirlos despues
        unidad = UNIDAD(POT);
       
        centena += '0';              //por ser idioma ASCII 48 significa 0
        decena += '0';               //de esta forma obtenemos numeros y no palabras
        unidad += '0';
        
        //POTENCIOMETRO
        LCD_CLEAR();               //aqui print para el LCD
        LCD_XY(1,0);
        LCD_STRING("S1:");
        LCD_XY(2,0);
        LCD_CHAR(centena);
        LCD_STRING(".");
        LCD_CHAR(decena);
        LCD_CHAR(unidad);
        
        RTC_PRINT();                //Funcion que imprime todas las variables del
                                    //RTC en el LCD
    }
    return;
}

void setup(void){
    //oscilador a 8M Hz
    OSCILLATOR(1);
    
    ANSEL = 0x00;
    ANSELH = 0x00;

    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;
    
    TRISB = 0b00000011;     //RB1 y RB0 entradas
    PORTB = 0x00;
    
    //Config. PULL UP
    IOC_INT(0b00000011);     //pines 1 y 2 se realizaran la interrupcion
    
    POT = 0;                //valor de variables iniciales
    unidad = 0;
    decena = 0;
    centena = 0;
    
    seg = 0;
    min = 0;
    hora= 0;
    dia = 0;
    mes = 0;
    year= 0;

    select = 0;
    parametro = 0;

    LCD_INIT();                     //iniciar configuracion LCD
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}

void RTC_PRINT (void){

    uint8_t seg1,min1,hora1;        //Se usan estas variables para que no
    uint8_t dia1,mes1,year1;        //reescribamos las principales y cause 
                                    //problemas o algun conflicto despues
    seg1 = BCD_DECIMAL(seg);
    min1 = BCD_DECIMAL(min);
    hora1 = BCD_DECIMAL(hora);      //convertimos de BCD a decimal     
    dia1 = BCD_DECIMAL(dia);
    mes1 = BCD_DECIMAL(mes);
    year1 = BCD_DECIMAL(year); 
    
    Time[0] = hora1/10 + '0';       //Separamos los decimales y unidades
    Time[1] = hora1 % 10 + '0';     //y lo guardamos en las tablas estaticas
    Time[3] = min1 / 10 + '0';
    Time[4] = min1 % 10 + '0';
    Time[6] = seg1/10 + '0';
    Time[7] = seg1 % 10 + '0';
    
    Date[0] = dia1/10 + '0';
    Date[1] = dia1 % 10 + '0';
    Date[3] = mes1/10 + '0';
    Date[4] = mes1 % 10 + '0';
    Date[8] = year1/10 + '0';
    Date[9] = year1 % 10 + '0';
    
    LCD_XY(1,8);                    //Imprimimos para LCD
    LCD_STRING(Time);
    LCD_XY(2,6);
    LCD_STRING(Date);
    __delay_ms(100);

}

unsigned short RTC_READ(unsigned I2C,unsigned d){
    unsigned short RTC;
    
    I2C_Master_Start();                 //Funcion para leer RTC y guardar 
    I2C_Master_Write(I2C);              //los datos usando comunicacion I2C
    I2C_Master_Write(d);
    I2C_Master_Stop();
    __delay_ms(10);
       
    I2C_Master_Start();
    I2C_Master_Write(I2C + 0x01);
    RTC = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(10);

    return RTC;
}

void Parpadeo(void){
    uint8_t j = 0;
    while(j>100 && PUSH1 && PUSH2){ //Tipo de delay para el parpadeo
        j++;
        __delay_ms(5);
    }
}


uint8_t DECIMAL_BCD(uint8_t num){       //convercion decimal a BCD
    return (((num/10)<<4) + (num % 10));
}

uint8_t BCD_DECIMAL(uint8_t num){       //convercion BCD a decimal
    return ((num >> 4)*10 + (num & 0x0f));
}


void RTC_WRITE(unsigned I2C,unsigned d,char parametro){
    
    I2C_Master_Start();             //Funcion usando comunicacion I2C para
    I2C_Master_Write(I2C);          //reescribir el RTC
    I2C_Master_Write(d);
    I2C_Master_Write(parametro);
    I2C_Master_Stop();
    __delay_ms(20);
    
}
 
uint8_t PAM(uint8_t parametro,uint8_t rtc){
    if(!PUSH1){
        if(ANTI_REBOTE())                   //RTC envia sus datos en lenguaje
            parametro = BCD_DECIMAL(rtc);   //BCD, para que no alla conflicto
        
    }                                   //causa efecto de precionar PUSH cierto tiempo
    return parametro;                   //parametro = rtc, si no rtc = parametro = 0 u dato anterior
}
uint8_t CONFIGURAR_v2(uint8_t x, uint8_t y, uint8_t parametro,uint8_t rtc){
    rtc = DECIMAL_BCD(parametro);      //se convierte a decimal
    
    LCD_XY(x,y);                //funcion para causar parpadeo en el LCD
    Parpadeo();
    LCD_STRING("  ");
    Parpadeo();
    return rtc;
    
    
}

__bit ANTI_REBOTE(void){
    uint8_t contador = 0;
    for(uint8_t i=0;i<5;i++){       //Funcion para evitar antirebote
        if(!PUSH1 || !PUSH2)        //Mas que todo utilizado para el PUSH1
            contador++;
        __delay_ms(10);
    }
    if(contador >2)
        return 1;
    else
        return 0;
}