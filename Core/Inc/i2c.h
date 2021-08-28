/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Marco, Roldan L.
  * @version v1.0
  * @date    August 27, 2021
  * @brief   I2C peripheral driver
  *   
  *          The I2C APIs present here follows the transfer sequence for reading 
  *          and writing described in the reference manual (RM0008).
  * 
  *          Device used: Bluepill (STM32F103C8)
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

#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

/* Own address used when in SLAVE mode */
#define STM32F1_SLV_ADDR            ( 0x5C )


typedef enum
{
    SLAVE = 0,
    MASTER = !SLAVE
} i2cMode_t;



/**
 * @brief    Initializes I2C1 and its GPIO
 * @param    none
 * @retval   none
 */
void i2c_init(void);



/**
 * @brief    Issue a start condition. When this function is
 *           called without calling stop first then this will
 *           be treated as a restart condition.
 * @param    none
 * @retval   none
 */
void i2c_start(void);



/**
 * @brief    Issue a stop condition to release the line
 * @param    none
 * @retval   none
 */
void i2c_stop(void);



/**
 * @brief    This function is called after issuing a start condition,
 *           this initiates the communication to slave device.
 * @param    slave_addr_rw: pre-shifted slave address and pre-appended RnW bit
 * @retval   none
 */
void i2c_request(uint8_t slave_addr_rw);



/**
 * @brief    Transmit a byte of data
 * @param    data: 1 byte data to be transmitted
 * @retval   none
 */
void i2c_write(uint8_t data);



/**
 * @brief    Transmit a N byte of data
 * @param    mode: MASTER or SLAVE transmitter
 * @param    data_bytes: number of bytes to transmit
 * @param    data_buffer: pointer to array where data are stored
 * @retval   none
 */
void i2c_write_burst(i2cMode_t mode, uint8_t data_bytes, uint8_t *data_buffer);



/**
 * @brief    Receives a byte of data
 *           Note: Stop condition is not required to call explicitly
 *           after each call to this function. This receiving sequence
 *           handles it already.
 * @param    none
 * @retval   1 byte of data from slave
 */
uint8_t i2c_read(void);



/**
 * @brief    Receives N bytes of data (N must be >= 2)
 *           Note: Stop condition is not required to call explicitly
 *           after each call to this function. This receiving sequence
 *           handles it already.
 * @param    mode: MASTER or SLAVE receiver
 * @param    data_bytes: number of bytes to receive. When in SLAVE mode
 *                       this parameter is ignored.
 * @param    data_buffer: pointer to array where data will be stored
 * @retval   none
 */
void i2c_read_burst(i2cMode_t mode, uint8_t data_bytes, uint8_t *data_buffer);


#endif