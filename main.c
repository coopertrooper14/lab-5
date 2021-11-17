// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "ADC.h"
#include "serial.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

unsigned int data;
unsigned int int_part;
unsigned int frac_part;

void main(void) {
    
    LCD_Init();  //Initialise LCD screen
    initUSART4();  //Initialise serial monitor
    ADC_init();  //Initialise ADC
    
    char buf[10];
    
    //data = getCharSerial4();  //Set data variable to the most recent character typed
    //sendCharSerial4(data);  //Send data to serial monitor
    //LCD_sendbyte(data, 1);  //Send data to LCD

    //data = ADC_getval();
    //sendStringSerial4(data);
    //sendCharSerial4(data);

    while (1) {

        int_part=ADC_getval()/77; // i.e. 255 / 5V = 51
        frac_part=(ADC_getval()*100)/77 - int_part*100;
        sprintf(buf,"%d.%02d",int_part,frac_part);
        LCD_sendstring(buf);
        sendStringSerial4(buf);

        __delay_ms(200);

        LCD_sendbyte(0b00000001, 0);

        __delay_ms(200);
    }
}
    
    