/**
  ******************************************************************************
  * @file    lcd.h
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 24, 2021
  * @brief   This file contains all the APIs for the 16x2 LCD (HD44780) firmware 
  *          library.
  ******************************************************************************
*/

#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"
#include <stdint.h>


/**
 * ******************************************************************************
 * Port Mapping:
 * 
 * LCD      GPIO        I2C
 * RS       PA1         I2C1 - SCL/SDA = PB6/PB7
 * RW       PA2         I2C2 - SCL/SDA = PB10/PB11 (not implemented)
 * EN       PA3
 * D4       PA4
 * D5       PA5
 * D6       PA6
 * D7       PA7
 * ******************************************************************************
 * Configuration Guide:
 * 
 * Setting macro to 1 enables it, 0 otherwise
 * 
 * USE_LCD_I2C                      set this to 1 to drive the LCD with I2C.
 *                                  Requires PCF8574 connected to LCD.
 * 
 * LCD_SLAVE_ADDR                   7-bit I2C address, default is 0x27
 * ******************************************************************************
 */


#define USE_LCD_I2C                 1

#if ( USE_LCD_I2C )
    #define LCD_SLAVE_ADDR          0x27
    #define LCD_SLAVE_W_ADDR        ( LCD_SLAVE_ADDR << 1 )
#endif



/* LCD APIs */

/**
 * @brief    LCD function to configure the pins and execute the initialization sequence
 * @param    none
 * @retval   none
 */
void lcd_init(void);


/**
 * @brief    LCD function to clear the entire display and sets the cursor to row 1, col 1
 * @param    none
 * @retval   none
 */
void lcd_clear(void);


/**
 * @brief    LCD Function to move the cursor on the display
 * @param    row: First row (1), second row (2)
 * @param    col: any value from 1-16
 * @retval   none
 */
void lcd_goto_xy(uint8_t row, uint8_t col);


/**
 * @brief    LCD function to control the display elements
 * @param    display : Enables (1) or disables (0) the character display
 * @param    cursor  : Enables (1) or disables (0) the lcd cursor
 * @param    blinking: Enables (1) or disables (0) the blinking of next character position
 * @retval   none
 */
void lcd_display_ctrl(uint8_t display, uint8_t cursor, uint8_t blinking);


/**
 * @brief    LCD function to print a string of characters to LCD
 * @param    str: pointer to array of characters
 * @retval   none
 */
void lcd_print_string(char *str);


/**
 * @brief    LCD function to shift the entire display
 * @param    dir: To the right (1), to the left (0)
 * @retval   none
 */
void lcd_shift_display(uint8_t dir);


#if ( USE_LCD_I2C )

/**
 * @brief    LCD function to turn on/off the backlight
 * @param    state: Off (0), On (1)
 * @retval   none
 */
void lcd_backlight(uint8_t state);

#endif



#endif /* __LCD_H */