
#include "lcd_i2c.h"

extern I2C_HandleTypeDef hi2c1;

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01

static void LCD_Send(uint8_t data)
{
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, 100);
}

static void LCD_Send_4Bit(uint8_t data)
{
    LCD_Send(data | LCD_BACKLIGHT | LCD_ENABLE);
    HAL_Delay(1);
    LCD_Send((data | LCD_BACKLIGHT) & ~LCD_ENABLE);
}

static void LCD_Send_Command(uint8_t cmd)
{
    LCD_Send_4Bit(cmd & 0xF0);
    LCD_Send_4Bit((cmd << 4) & 0xF0);
}

static void LCD_Send_Data(uint8_t data)
{
    LCD_Send_4Bit((data & 0xF0) | LCD_RS);
    LCD_Send_4Bit(((data << 4) & 0xF0) | LCD_RS);
}

void LCD_Init(void)
{
    HAL_Delay(50);
    LCD_Send_Command(0x33);
    LCD_Send_Command(0x32);
    LCD_Send_Command(0x28);
    LCD_Send_Command(0x0C);
    LCD_Send_Command(0x06);
    LCD_Send_Command(0x01);
    HAL_Delay(5);
}

void LCD_Clear(void)
{
    LCD_Send_Command(0x01);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? 0x80 : 0xC0;
    LCD_Send_Command(addr + col);
}

void LCD_Send_String(char *str)
{
    while (*str)
        LCD_Send_Data(*str++);
}
