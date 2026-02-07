/*
 * lcd_i2c.h
 *
 *  Created on: Jan 1, 2026
 *      Author: Admin
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_
#include "stm32f4xx_hal.h"

#define LCD_I2C_ADDR (0x27 << 1)

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Send_String(char *str);


#endif /* INC_LCD_I2C_H_ */
