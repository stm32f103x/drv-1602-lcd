/**
  ******************************************************************************
  * @file    lcd.c
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 21, 2021
  * @brief   1602 LCD Driver (HD44780U). This driver configures the LCD in 4-bit
  *          interface which either uses bit banging or with I2C which requires
  *          PCF8574 I/O expander connected to the LCD.
  *          See lcd.h for configuration.
  *
  *          Device used: Bluepill (STM32F103C8x)
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


#include "lcd.h"


static void lcd_gpio(void);
static void lcd_data_line(uint8_t data);
static void lcd_print_char(char data);
static void lcd_cmd(uint8_t cmd);
static void lcd_busy_wait(uint32_t delay);

#if ( USE_LCD_I2C )

#include "i2c.h"

static void lcd_i2c_cmd(uint8_t data);
static uint8_t backlight_state = 0x08;

#else

static void lcd_rs_pin(uint8_t rs);
static void lcd_rw_pin(uint8_t rw);
static void lcd_en_pin(void);

#endif



/**
 * @brief    LCD function to configure the pins and execute the initialization sequence
 * @param    none
 * @retval   none
 */
void lcd_init(void)
{
    /* Initialize LCD GPIO pins */
    lcd_gpio();

    #if ( USE_LCD_I2C )

    /* initialize the i2c peripheral */
    // lcd_i2c_config();
    i2c_init();


    /* LCD initialization sequence */
    lcd_busy_wait(100);

    lcd_cmd(0x03);
    lcd_busy_wait(20);

    lcd_cmd(0x03);
    lcd_busy_wait(300);

    lcd_cmd(0x03);
    lcd_cmd(0x02);

    #else

    /* LCD initialization sequence */
    lcd_busy_wait(100);

    lcd_rs_pin(0);
    lcd_rw_pin(0);
    lcd_data_line(0x03);
    lcd_busy_wait(20);

    lcd_data_line(0x03);
    lcd_busy_wait(300);

    lcd_data_line(0x03);
    lcd_data_line(0x02);

    #endif

    /* Function set */
    lcd_cmd(0x28);
    lcd_busy_wait(1);

    /* display off */
    lcd_display_ctrl(1, 0, 0);
    lcd_busy_wait(1);

    /* display clear */
    lcd_clear();

    /* entry mode set */
    lcd_cmd(0x06);
    lcd_busy_wait(1);

}



#if ( USE_LCD_I2C )

/**
 * @brief    LCD function to turn on/off the backlight
 * @param    state: Off (0), On (1)
 * @retval   none
 */
void lcd_backlight(uint8_t state)
{
    if(state)
    {
        backlight_state = 0x08;
        lcd_i2c_cmd(0x00);
    }
    else
    {
        backlight_state = 0x00;
        lcd_i2c_cmd(0x00);
    }
}

#endif



/**
 * @brief    LCD function to clear the entire display and sets the cursor to row 1, col 1
 * @param    none
 * @retval   none
 */
void lcd_clear(void)
{
    lcd_cmd(0x01);
    lcd_busy_wait(4);
}



/**
 * @brief    LCD Function to move the cursor on the display
 * @param    row: First row (1), second row (2)
 * @param    col: any value from 1-16
 * @retval   none
 */
void lcd_goto_xy(uint8_t row, uint8_t col)
{
    col--;

    switch(row)
    {
    case 1:
        lcd_cmd((col |= 0x80));
        break;
    case 2:
        lcd_cmd((col |= 0xC0));
        break;
    default:
        break;
    }
}



/**
 * @brief    LCD function to control the display elements
 * @param    display : Enables (1) or disables (0) the character display
 * @param    cursor  : Enables (1) or disables (0) the lcd cursor
 * @param    blinking: Enables (1) or disables (0) the blinking of next character position
 * @retval   none
 */
void lcd_display_ctrl(uint8_t display, uint8_t cursor, uint8_t blinking)
{
    uint8_t tmp = 0x08;

    if(display)
    {
        tmp |= (1U << 2);
    }
    if(cursor)
    {
        tmp |= (1U << 1);
    }
    if(blinking)
    {
        tmp |= (1U << 0);
    }
    lcd_cmd(tmp);
}



/**
 * @brief    LCD function to shift the entire display
 * @param    dir: 1 to shift display to right, 0 to left
 * @retval   none
 */
void lcd_shift_display(uint8_t dir)
{
    if(dir)
    {
        lcd_cmd(0x1C);
    }
    else
    {
        lcd_cmd(0x18);
    }
}



/**
 * @brief    LCD function to print a string of characters to LCD
 * @param    str: pointer to array of characters
 * @retval   none
 */
void lcd_print_string(char *str)
{
    for(uint8_t i = 0; str[i] != '\0'; i++)
    {
        lcd_print_char(str[i]);
    }
}



/**
 * @brief    Static function to print a single character to LCD
 * @param    ch: character to be printed
 * @retval   none
 */
static void lcd_print_char(char ch)
{
    #if ( USE_LCD_I2C )

    lcd_data_line( (ch & 0xF0) | 0x01 );
    lcd_data_line( (ch << 4) | 0x01 );

    #else

    lcd_rs_pin(1);
    lcd_rw_pin(0);
    lcd_data_line(ch >> 4);
    lcd_data_line(ch & 0x0f);

    #endif
}



/**
 * @brief    Function to configure PA<7:1> to be used by the LCD
 *           or PB<7:6> for SDA/SCL if I2C is used
 * @param    none
 * @retval   none
 */
static void lcd_gpio(void)
{
    #if ( USE_LCD_I2C )

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= ( RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN );

    /* PB6 - SCL, PB7 - SDA  */
    /* Alternate function output Open-drain, 50 MHz */

    GPIOB->CRL &= ~( GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 |
                     GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0 );

    GPIOB->CRL &= ~( GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0 |
                     GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 );

    GPIOB->CRL |= ( GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0 );
    GPIOB->CRL |= ( GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 );

    GPIOB->BSRR |= ( GPIO_BSRR_BS6 | GPIO_BSRR_BS7 );

    #else

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    for(uint8_t i = 0; i < 28; i += 4)
    {
        /* Clear PA<7:1> */
        GPIOA->CRL &= ~(0x0FUL << (4UL + i));

        /* Set the required bits */
        /* General purpose output push-push 50 MHz */
        GPIOA->CRL |= (0x03UL << (4UL + i));
    }

    for(uint8_t i = 0; i < 7; i++)
    {
        /* Reset PA<7:1> */
        GPIOA->BSRR |= (1 << (17UL + i));
    }

    #endif
}



/**
 * @brief    Function to issue a command to LCD
 * @param    cmd: 8 bit data command. See the datasheet for more information.
 * @retval   none
 */
static void lcd_cmd(uint8_t cmd)
{
    #if ( USE_LCD_I2C )

    lcd_data_line(cmd & 0xF0);
    lcd_data_line( (cmd << 4));

    #else

    lcd_rs_pin(0);
    lcd_rw_pin(0);
    lcd_data_line(cmd >> 4);
    lcd_data_line(cmd & 0x0f);

    #endif
}



/**
 * @brief    Function that extracts the lower nibble of 8-bit data
 *           and handles the latching of data by toggling the EN pin.
 *           When in I2C mode this function only handles the toggling
 *           of EN pin by shifting values to bit 3 (bit position of EN
 *           pin)
 * @param    data: 8-bit data where the first nibble will be extracted
 * @retval   none
 */
static void lcd_data_line(uint8_t data)
{
    #if ( USE_LCD_I2C )

    lcd_i2c_cmd(data | (1 << 2));
    lcd_i2c_cmd(data | (0 << 2));
    lcd_busy_wait(300);

    #else

    for(uint8_t i = 0; i < 4; i++ )
    {
        uint8_t tmp = ((data >> i) & 0x01);

        if(tmp)
        {
            GPIOA->BSRR |= (1 << (4 + i));
        }
        else
        {
            GPIOA->BSRR |= (1 << (20 + i));
        }
    }
    lcd_en_pin();

    #endif
}



#if ( USE_LCD_I2C )

/**
 * @brief    Function that directly communicate to PCF8574
 * @param    data: 8-bit data to be transmitted via I2C line
 * @retval   none
 */
static void lcd_i2c_cmd(uint8_t data)
{
    i2c_start();
    i2c_request(LCD_SLAVE_W_ADDR);
    i2c_write(data | backlight_state);
    i2c_stop();
}

#else

/**
 * @brief    Static function that controls the state of RS pin of LCD
 * @param    rw: 1 - High, 0 - Low
 * @retval   none
 */
static void lcd_rs_pin(uint8_t rs)
{

    if(rs)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS1;
    }
    else
    {
        GPIOA->BSRR |= GPIO_BSRR_BR1;
    }

}



/**
 * @brief    Static function that controls the state of RW pin of LCD
 * @param    rw: 1 - High, 0 - Low
 * @retval   none
 */
static void lcd_rw_pin(uint8_t rw)
{
    if(rw)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS2;
    }
    else
    {
        GPIOA->BSRR |= GPIO_BSRR_BR2;
    }
}



/**
 * @brief    Static function that controls the EN pin of LCD
 * @param    none
 * @retval   none
 */
static void lcd_en_pin(void)
{
    GPIOA->BSRR |= GPIO_BSRR_BS3;
    lcd_busy_wait(100);
    GPIOA->BSRR |= GPIO_BSRR_BR3;
    lcd_busy_wait(100);
}

#endif



/**
 * @brief    Static function to implement blocking delay
 * @param    delay: a value of 100 = approx 50us
 * @retval   none
 */
static void lcd_busy_wait(uint32_t delay)
{
    delay = delay * 40ul;
    for(uint32_t i = 0; i < delay; i++);
}
