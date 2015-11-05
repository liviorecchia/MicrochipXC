/*
 * olimex16x2.c
 * based on OLIMEX's LCD16x2.cpp
 * Copyright 2013 OLIMEX LTD <support@olimex.com>
 * Copyright 2015 Recchia Livio <recchialivio@libero.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITy or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * you should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <xc.h>
#include "i2c.h"
#include "olimex16x2.h"
#include "system.h" 
// Define clock frequency like the example below if file "config.h" is not available
// #define _XTAL_FREQ      4000000L

char LCD16x2x;
char LCD16x2y;

/**
 * Read the id of the shield.
 * @return      If read value match BOARD_ID(0x65) the method return 1,
 * otherwise - 0.
 */

char LCD16x2getID(){
    char id = 0;
    I2CStart();
    I2CSend((LCD16x2ADDRESS<<1)|0x00);
    I2CSend(LCD16x2GET_ID);
    I2CStop();
    I2CRestart();
    I2CSend((LCD16x2ADDRESS<<1)|0x01);
    id=I2CRead();
    I2CStop();
    return id;
}

void LCD16x2lcdSetBacklight(char value){
	I2CStart();
    	I2CSend(LCD16x2ADDRESS<<1);
	I2CSend(LCD16x2SET_BL);
	I2CSend(value);
	I2CStop();
}
void LCD16x2uartEnable(char state){
    char en;
    if(state == 1)
        en = 0x01;
    else
        en = 0x00;

    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2UART_EN);
    I2CSend(en);
    I2CStop();
}

char LCD16x2getFirmwareVersion(){
    char firm = 0;
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2GET_FRM);
    I2CStop();
    I2CRestart();
    I2CSend((LCD16x2ADDRESS<<1)|0x01);
    firm=I2CRead();
    I2CStop();
    return firm;
}
/**
 * Set direction of GPIO.
 * @param pin   The pin number according to schematic: GPIO1, GPIO2, etc...
 * @param direction     The direction of the GPIO: OUTPUT or INPUT.
 */
void LCD16x2pinMode(char pin, char direction){
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2SET_TRIS);
    I2CSend(pin);
    I2CSend(!direction);
    I2CStop();
}

/**
 * If pin is set as output this method define the level of the GPIO.
 * If pin is input - it enables charernal pull-up resistor (only available
 * for PORTB pins).
 * @param pin   The number of the desired GPIO: GPIO1, GPIO2, etc...
 * @param level The output level: HIGH or LOW
 */
void LCD16x2digitalWrite(char pin, char level){
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2SET_LAT);
    I2CSend(pin);
    I2CSend(level);
    I2CStop();
}

/**
 * Read the state of individual GPIO, if configured as input.
 * @param pin   The number of the GPIO: GPIO1, GPIO2, etc...
 * @return      If input level is high - 1, else - 0.
 */
char LCD16x2digitalRead(char pin){
    char port;
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2GET_PORT);
    I2CSend(pin);
    I2CStop();
    I2CRestart();
    I2CSend((LCD16x2ADDRESS<<1)|0x01);
    port = I2CRead();
    I2CStop();
    return port;
}

/**
 * Read the state of the 4 buttons.
 * @return      Bitmask with the 4 values: LSB - BUT1, MSB - BUT4
 * Data structure: [XXXX4321]
 * Button 1 is the first on the left
 */

char LCD16x2readButtons(){
    char buttons;
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2GET_BUT);
    I2CStop();
    I2CRestart();
    I2CSend((LCD16x2ADDRESS<<1)|0x01);
    buttons = I2CRead();
    I2CStop();
    return buttons;
}

/**
 * Clear the LCD screen.
 */
void LCD16x2lcdClear(){
    I2CStart();
    I2CSend(LCD16x2ADDRESS<<1);
    I2CSend(LCD16x2LCD_CLR);
    I2CStop();
    LCD16x2x=0;
    LCD16x2y=1;
    __delay_ms(100);
}

/**
 * Position the cursor of the LCD to a given x and y coordinates.
 * @param x     x coordinate
 * @param y     y coordinate
 */
void LCD16x2lcdGoToxy(char x, char y){
    if(x > 16) return;
    LCD16x2x=x;
    //else        LCD16x2x = LCD16x2x - 1;
    if(LCD16x2y > 1) return;
    LCD16x2y=y+1;
}

char LCD16x2strlen(char* asd){
    char len=0;
    while(asd[len]!=0x00){
        len++;
    }
    return len;
}


void LCD16x2putChar(char c){
    if(c=='\n'){
        LCD16x2x=0;
        LCD16x2y++;
        return;
    }else{
        I2CStart();
        I2CSend(LCD16x2ADDRESS<<1);
        I2CSend(LCD16x2LCD_WR);
        I2CSend(LCD16x2y);
        I2CSend(LCD16x2x);
        I2CSend(c);
        I2CStop();
        __delay_ms(20);
        LCD16x2x++;
        if(LCD16x2x > 15){
            LCD16x2x = 0;
            LCD16x2y++;
            if(LCD16x2y > 2)
                return;
        }
    }
}

/**
 * Write string to the LCD screen.
 * @param string        String to be written.
 */
void LCD16x2lcdWrite(char* string){
    char len;
    len = LCD16x2strlen(string);
    for(char i = 0; i < len; i++){
        LCD16x2putChar(string[i]);
    }
}
