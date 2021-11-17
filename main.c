// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "serial.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

unsigned int data;

void main(void) {
    
    LCD_Init();
    initUSART4();
    //LCD_sendbyte(0b10001111, 0);
    //    __delay_us(40);
    
    while (1){
        data = getCharSerial4();
        LCD_sendbyte(data, 1);
    //    __delay_ms(100); //Enough delay to read text moving along
    //    LCD_sendbyte(0b00011000, 0);
    //    __delay_ms(100);
    }
}