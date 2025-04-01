/*
 * ecran.h
 *
 *  Created on: Feb 11, 2025
 *      Author: lijia
 */
#include <stdint.h>


#ifndef INC_ECRAN_H_
#define INC_ECRAN_H_

#define LCD_ADDRESS  0x3E // Adresse I2C du LCD
#define RGB_ADDRESS  0x62 // Adresse I2C du rétroéclairage

void LCD_SendCommand(uint8_t cmd);

void LCD_SendData(uint8_t data);

void LCD_Init();
void LCD_Clear();
void LCD_PrintFormatted(const char *format, ...);
void LCD_Print(char *str);

void LCD_SetRGB(uint8_t r, uint8_t g, uint8_t b);

#endif /* INC_ECRAN_H_ */
