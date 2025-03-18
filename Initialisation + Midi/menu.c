/*
 * menu.c
 *
 *  Created on: Mar 11, 2025
 *      Author: lijia
 */
#include "main.h"
#include "menu.h"
#include "ecran.h"

#include "init.h"

#include "stdio.h"
#include "string.h"

// Définition des menus
const char *menu_principal[] = {"Musics", "BPM", "Color","vide"};

const char *menu_Musics[] = {"Musics 1", "2", "3","4"};
const char *menu_BPM[] = {"90", "50", "30","10"};
const char *menu_Color[] = {"bleu", "blanc", "rouge","vert"};
// Variables de navigation
int menu_index = 0;
int menu_selected = 0; // 0 = menu principal, 1 = sous-menu
int sous_menu_index = 0;



void Navigation() {
	LCD_Clear();
    LCD_DisplayMenu();

    while (1) {
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {  // Bouton Entrer
        	menu_selected=1;
            if (menu_index == 0){
                LCD_Clear();
                LCD_DisplaySousMenu();

            }
            if (menu_index == 1){
                LCD_Clear();
                LCD_DisplaySousMenu();


                        }
            if (menu_index == 2){
                LCD_Clear();
                LCD_DisplaySousMenu();


                        }
            HAL_Delay(200);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET && menu_selected==1) {  // Bouton Retour
            menu_selected = 0;
            LCD_Clear();
            LCD_DisplayMenu();
            HAL_Delay(500);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET && menu_selected==0) {  // Défilement Haut
            if (menu_index > 0){
            	menu_index--;
            	LCD_Clear();
				LCD_DisplayMenu();
				HAL_Delay(300);
            }
        }else {
        	if( sous_menu_index>0){
        	sous_menu_index--;
        	LCD_Clear();
        	LCD_DisplaySousMenu();
        					HAL_Delay(300);
        }
            }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET && menu_selected==0) { // Défilement Bas
            if (menu_index < 2){
            	menu_index++;
            	LCD_Clear();
				LCD_DisplayMenu();
				HAL_Delay(300);
            }

        }else { if( sous_menu_index>0){
        	sous_menu_index++;
        	LCD_Clear();
        	LCD_DisplaySousMenu();
        					HAL_Delay(300);

        }

    }
}}

void LCD_DisplayMenu() {
	LCD_PrintFormatted((char *)menu_principal[menu_index]);  // Première ligne
    LCD_SendCommand(0xC0); // Passer à la ligne suivante
    LCD_PrintFormatted((char *)menu_principal[menu_index + 1]); // Deuxième ligne
}

void LCD_DisplaySousMenu() {

	if (menu_index==0){
		LCD_PrintFormatted((char *)menu_Musics[sous_menu_index]);  // Première ligne
		    LCD_SendCommand(0xC0); // Passer à la ligne suivante
		    LCD_PrintFormatted((char *)menu_Musics[sous_menu_index + 1]); // Deuxième ligne
	}

	else if (menu_index==1){
	LCD_PrintFormatted((char *)menu_BPM[sous_menu_index]);  // Première ligne
    LCD_SendCommand(0xC0); // Passer à la ligne suivante
    LCD_PrintFormatted((char *)menu_BPM[sous_menu_index + 1]); // Deuxième ligne
	}
	else {
	LCD_PrintFormatted((char *)menu_Color[sous_menu_index]);  // Première ligne
    LCD_SendCommand(0xC0); // Passer à la ligne suivante
    LCD_PrintFormatted((char *)menu_Color[sous_menu_index + 1]); // Deuxième ligne
}
}
