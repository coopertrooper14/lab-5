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
    
    LCD_Init();  //Initialise LCD screen
    initUSART4();  //Initialise serial monitor

    while (1){
        data = getCharSerial4();  //Set data variable to the most recent character typed
        sendCharSerial4(data);  //Send data to serial monitor
        LCD_sendbyte(data, 1);  //Send data to LCD
    }
}