/*
 * ecran.c
 *
 *  Created on: Feb 11, 2025
 *      Author: lijia
 */
#include "ecran.h"
#include <stdio.h>
#include <stdarg.h>

#include "main.h"

extern I2C_HandleTypeDef hi2c1;  // Déclaration du bus I²C

#define LCD_ADDR 0x3E << 1  // Adresse de l'écran
#define RGB_ADDR 0x62 << 1  // Adresse du rétroéclairage

void LCD_SendCommand(uint8_t cmd) {
    uint8_t data[2] = {0x80, cmd};  // 0x80 = Command Mode
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data, 2, HAL_MAX_DELAY);
}

void LCD_SendData(uint8_t data) {
    uint8_t temp[2] = {0x40, data};  // 0x40 = Data Mode
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, temp, 2, HAL_MAX_DELAY);
}

void LCD_Init() {
    HAL_Delay(50);
    LCD_SendCommand(0x38); // Mode 8-bit, 2 lignes, 5x8 dots
    LCD_SendCommand(0x39); // Activer les fonctions étendues
    LCD_SendCommand(0x14); // OSC fréquence réglée
    LCD_SendCommand(0x70); // Contraste
    LCD_SendCommand(0x56); // Alimentation et booster interne
    LCD_SendCommand(0x6C); // Attendre la stabilisation
    HAL_Delay(200);
    LCD_SendCommand(0x38); // Revenir en mode standard
    LCD_SendCommand(0x0C); // Affichage ON, curseur OFF
    LCD_SendCommand(0x01); // Effacer écran
    HAL_Delay(2);
}

void LCD_Print(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

void LCD_Clear() {
    LCD_SendCommand(0x01); // Commande pour effacer l'écran
    HAL_Delay(2);          // Petit délai pour éviter les bugs d'affichage
}

void LCD_PrintFormatted(const char *format, ...) {
    char buffer[32];  // Stocke la chaîne formatée
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    LCD_Print(buffer); // Envoie le texte à l’écran
}

void LCD_SetRGB(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDR, data, 2, HAL_MAX_DELAY);

    data[0] = 0x08;
    data[1] = 0xAA;
    HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDR, data, 2, HAL_MAX_DELAY);

    data[0] = 0x04;
    data[1] = r;
    HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDR, data, 2, HAL_MAX_DELAY);

    data[0] = 0x03;
    data[1] = g;
    HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDR, data, 2, HAL_MAX_DELAY);

    data[0] = 0x02;
    data[1] = b;
    HAL_I2C_Master_Transmit(&hi2c1, RGB_ADDR, data, 2, HAL_MAX_DELAY);
}
