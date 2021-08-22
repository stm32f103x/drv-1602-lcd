#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"
#include <stdint.h>


/**
 * Port Mapping:
 * RS - PA1
 * RW - PA2
 * EN - PA3
 * D4 - PA4
 * D5 - PA5
 * D6 - PA6
 * D7 - PA7
 */




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
 * @param    row: either 1 for first row, or 2 for second row
 * @param    col: any values from 1-16
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
void lcd_print_string(char *message);


/**
 * @brief    LCD function to shift the entire display
 * @param    dir: 1 to shift display to right, 0 to left
 * @retval   none
 */
void lcd_shift_display(uint8_t dir);




#endif /* __LCD_H */