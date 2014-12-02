/**
 * @file: 	hd44780_hal.c
 * @brief:	   
 * @date: 	8 paź 2014
 * @author: Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <hd44780_hal.h>

#include <avr/io.h>

/*
 * Ports and pins of the LCD
 */
#define LCD_DATA_PORT PORTC  ///< LCD data GPIO
#define LCD_DATA_DIR  DDRC   ///< LCD data direction register
#define LCD_DATA_PIN  PINC   ///< LCD data input register
#define LCD_CTRL_PORT PORTA  ///< LCD control GPIO
#define LCD_CTRL_DIR  DDRA   ///< LCD data direction register
#define LCD_CTRL_PIN  PINA   ///< LCD data input register

#define LCD_RS  (1<<5) ///< Register select pin
#define LCD_RW  (1<<6) ///< Read/write pin
#define LCD_E   (1<<7) ///< Enable pin

/*
 * We use the 4-bit interface
 */
#define LCD_D4  (1<<4) ///< Data 4 pin
#define LCD_D5  (1<<5) ///< Data 5 pin
#define LCD_D6  (1<<6) ///< Data 6 pin
#define LCD_D7  (1<<7) ///< Data 7 pin

/**
 * @brief Low level initalization of the LCD.
 */
void LCD_HAL_Init(void) {

  // Set LCD data pins initially as output
  LCD_HAL_DataOut();

  // Set control pins as output
  LCD_CTRL_DIR |= (LCD_RS | LCD_RW | LCD_E);
  // Clear all control signals initially
  LCD_CTRL_PORT &= ~(LCD_RS | LCD_RW | LCD_E);

}

void LCD_HAL_LowRS(void) {
  LCD_CTRL_PORT &= ~LCD_RS;
}
void LCD_HAL_HighRS(void) {
  LCD_CTRL_PORT |= LCD_RS;
}
void LCD_HAL_LowRW(void) {
  LCD_CTRL_PORT &= ~LCD_RW;
}
void LCD_HAL_HighRW(void) {
  LCD_CTRL_PORT |= LCD_RW;
}
void LCD_HAL_HighE(void) {
  LCD_CTRL_PORT |= LCD_E;

}
void LCD_HAL_LowE(void) {
  LCD_CTRL_PORT &= ~LCD_E;
}

/**
 * @brief Set data lines as output.
 */
void LCD_HAL_DataOut(void) {
  LCD_DATA_DIR |= (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
}

/**
 * @brief Set data lines as input with pull up
 */
void LCD_HAL_DataIn(void) {
  LCD_DATA_DIR &= ~(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
  LCD_DATA_PORT |= (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
}

/**
 * @brief This functions sets data on the data lines when writing.
 * @param data Data to write
 */
void LCD_HAL_Write(uint8_t data) {

  // set the various bits
  if (data & (1<<3))
    LCD_DATA_PORT |= LCD_D7;
  else
    LCD_DATA_PORT &= ~LCD_D7;

  if (data & (1<<2))
    LCD_DATA_PORT |= LCD_D6;
  else
    LCD_DATA_PORT &= ~LCD_D6;

  if (data & (1<<1))
    LCD_DATA_PORT |= LCD_D5;
  else
    LCD_DATA_PORT &= ~LCD_D5;

  if (data & (1<<0))
    LCD_DATA_PORT |= LCD_D4;
  else
    LCD_DATA_PORT &= ~LCD_D4;
}

/**
 * @brief Reads the data lines
 * @return Read data.
 */
uint8_t LCD_HAL_Read(void) {
  uint8_t result = 0;

  LCD_CTRL_PORT |= LCD_E;
  asm("nop");
  asm("nop");
  if(LCD_DATA_PIN & (LCD_D7))
    result |= (1<<3);
  if(LCD_DATA_PIN & (LCD_D6))
    result |= (1<<2);
  if(LCD_DATA_PIN & (LCD_D5))
    result |= (1<<1);
  if(LCD_DATA_PIN & (LCD_D4))
    result |= (1<<0);
  asm("nop");
  asm("nop");
  LCD_CTRL_PORT &= ~LCD_E;

  return result;
}
