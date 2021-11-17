#include <xc.h>
#include "LCD.h"

unsigned int highbits;
unsigned int lowbits;
unsigned int int_part;
unsigned int frac_part;

//Define easy to remember variable names to pin outputs.
#define RS LATCbits.LATC6
#define E LATCbits.LATC2
#define DB4 LATBbits.LATB3
#define DB5 LATBbits.LATB2
#define DB6 LATEbits.LATE3
#define DB7 LATEbits.LATE1

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
	//turn the LCD enable bit on
    
    E = 1;
	__delay_us(2); //wait a short delay
	//turn the LCD enable bit off again
    E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{ 
	//Set the data lines here (think back to LED array output)
    //Check each bit of the number and turn on the appropriate pins
    if (number & 0b0001) {DB4 = 1;} else {DB4 = 0;}
    if (number & 0b0010) {DB5 = 1;} else {DB5 = 0;}
    if (number & 0b0100) {DB6 = 1;} else {DB6 = 0;}
    if (number & 0b1000) {DB7 = 1;} else {DB7 = 0;}
    
    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);      //Delay 5uS
}


/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{ 
    // send high bits of Byte using LCDout function
    // send low bits of Byte using LCDout function
    unsigned char highbits;
    //Bit shift Byte to get the high bits.
    highbits = Byte >> 4;
    
    // set RS pin whether it is a Command (0) or Data/Char (1) using type argument
    if (type == 1) {RS = 1;} else {RS = 0;}
    
    //Send high and low bits one after another
    LCD_sendnibble(highbits);
    LCD_sendnibble(Byte);

    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{

    //Define LCD Pins as Outputs and
    //set all pins low (might be random values on start up, fixes lots of issues)
    
    //Set tris registers for all pins to outputs.
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE1 = 0;
    
    //Set all outputs to zero.
    RS = 0;
    E = 0;
    DB4 = 0;
    DB5 = 0;
    DB6 = 0;
    DB7 = 0;

    //Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
    // first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
	// after this use LCD_sendbyte to operate in 4 bit mode
    
    //Power on
    //Wait for more than 40ms after Vdd rises to 4.5V
    __delay_ms(41);
    
    LCD_sendnibble(0b0011);  //Function set
    __delay_us(40);  //Wait for more than 39us
    LCD_sendbyte(0b00101000,0);  //Function set
    __delay_us(40);  //Wait for more than 39us
    LCD_sendbyte(0b00101000,0);  //Function set
    __delay_us(38);  //Wait for more than 37us
    LCD_sendbyte(0b00001110,0);  //Display ON/OFF control
    __delay_us(38);  //Wait for more than 37us
    LCD_sendbyte(0b00000001,0);  //Display clear
    __delay_ms(1.55);  //Wait for more than 1.53ms
    LCD_sendbyte(0b00000110,0);  //Entry mode set
   
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{   
    LATEbits.LATE1 = 1;
    
    //Set to line 1
    //Send 0x80 to set line to 1 (0x00 ddram address)
    if (line == 1) {
        LCD_sendbyte(0b10000000, 0); 
        __delay_us(40);
    }
    
    //Set to line 2
    //Send 0xC0 to set line to 2 (0x40 ddram address)
        if (line == 2) {
        LCD_sendbyte(0b11000000, 0);
        __delay_us(40);
    }
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{
	//code here to send a string to LCD using pointers and LCD_sendbyte function
    
    unsigned int count = 0;
    
    //Count through each letter of the string using pointer and display each letter.
    while(*string != 0){
        LCD_sendbyte(*string++, 1);
       // count++;
    }
    //When count passes 16 scroll the screen using LCD_scroll()
    unsigned int i = 0;
    //for (i=0; i<(count-16); i++) {LCD_scroll();}
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_scroll(void)
{
	//code here to scroll the text on the LCD screen
    __delay_ms(500); //Enough delay to read text moving along
    LCD_sendbyte(0b00011000, 0);
    __delay_us(50);
}