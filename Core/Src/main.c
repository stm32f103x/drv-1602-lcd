/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 21, 2021
  * @brief   1602 LCD Driver (HD44780U) test code
  ******************************************************************************
  *
  * Copyright (C) 2021  Marco, Roldan L.
  * 
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * any later version.
  * 
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see https://www.gnu.org/licenses/gpl-3.0.en.html.
  * 
  * 
  * https://github.com/rmarco30
  * 
  ******************************************************************************
**/



/**
  ******************************************************************************
  * @file    main.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 21, 2021
  * @brief   
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "lcd.h"

#define DELAY_VAL       10000000

void delay(uint32_t del)
{
    while(del != 0)
    {
      del--;
    }
}


int main()
{
    lcd_init();
    lcd_print_string("16x2 LCD Test");
    delay(DELAY_VAL);
    lcd_clear();

    while(1)
    {
        lcd_print_string("ROW 1");
        delay(DELAY_VAL);
        lcd_clear();

        lcd_goto_xy(2, 1);
        lcd_print_string("ROW 2");
        delay(DELAY_VAL);
        lcd_clear();

        lcd_print_string("Display control");
        lcd_goto_xy(2, 1);
        lcd_print_string("test");
        for(uint8_t i = 0; i < 2; i++)
        {
            lcd_display_ctrl(1, 0, 0);
            delay(DELAY_VAL);
            lcd_display_ctrl(0, 0, 0);
            delay(DELAY_VAL);
        }
        lcd_clear();

        lcd_print_string("Display cursor");
        lcd_goto_xy(2, 1);
        lcd_print_string("test");
        lcd_display_ctrl(1, 1, 0);
        delay(DELAY_VAL);
        lcd_clear();

        lcd_print_string("Blinking cursor");
        lcd_goto_xy(2, 1);
        lcd_print_string("test");
        lcd_display_ctrl(1, 1, 1);
        delay(DELAY_VAL);
        lcd_clear();
        lcd_display_ctrl(1, 0, 0);

        lcd_print_string("Shift right >>");
        for(uint8_t i = 0; i < 16; i++)
        {
            lcd_shift_display(1);
            delay(1000000);
        }
        lcd_clear();

        lcd_goto_xy(1, 3);
        lcd_print_string("<< Shift left");
        for(uint8_t i = 0; i < 16; i++)
        {
            lcd_shift_display(0);
            delay(1000000);
        }
        lcd_clear();

        lcd_print_string("Back light test");
        for(uint8_t i = 0; i < 10; i++)
        {
            lcd_backlight(0);
            delay(1000000);
            lcd_backlight(1);
            delay(1000000);
        }
        lcd_clear();
    }
}