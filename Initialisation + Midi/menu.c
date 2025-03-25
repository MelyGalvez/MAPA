/*
 * menu.c
 *
 *  Created on: Mar 11, 2025
 *      Author: lijia
 */
#include "main.h"
#include "menu.h"
#include "ecran.h"
#include "midi_parser.h"
#include "init.h"

#include "stdio.h"
#include "string.h"

// Définition des menus
const char *menu_principal[] = {"Musics", "BPM", "Color"," "};

const char *menu_Musics[] = {"Musics 1", "Musics 2", "Musics 3","Musics 4"};
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
    	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET && menu_selected==1 && menu_index==0){
		  process_midi((uint16_t *) midi_data, midi_data_length);}

        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {  // Bouton de droite: Entrer
        	 if (menu_selected<1){
        		 menu_selected++;
        	 }
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

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET && menu_selected==1) {  // Bouton de gauche: Retour
            menu_selected--;
            sous_menu_index=0;
            LCD_Clear();
            LCD_DisplayMenu();
            HAL_Delay(500);
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET ) {  //Bonton du haut: Défilement Haut
            if ( menu_selected==0){
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
        }
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_RESET) { // Bonton du bas: Défilement Bas
            if(menu_selected==0){
            	if (menu_index < sizeof(menu_principal) / sizeof(menu_principal[0]) - 2){
            	menu_index++;
            	LCD_Clear();
				LCD_DisplayMenu();
				HAL_Delay(300);
            }
            } else {
                   // Limite de sous-menu pour Music
                   if (menu_index == 0 && sous_menu_index < sizeof(menu_Musics) / sizeof(menu_Musics[0]) - 1) {
                       sous_menu_index++;
                       LCD_Clear();
                       LCD_DisplaySousMenu();
                       HAL_Delay(300);
                   }
                   // Limite de sous-menu pour BPM
                   else if (menu_index == 1 && sous_menu_index < sizeof(menu_BPM) / sizeof(menu_BPM[0]) - 1) {
                       sous_menu_index++;
                       LCD_Clear();
                       LCD_DisplaySousMenu();
                       HAL_Delay(300);
                   }
                   // Limite de sous-menu pour Color
                   else if (menu_index == 2 && sous_menu_index < sizeof(menu_Color) / sizeof(menu_Color[0]) - 1) {
                       sous_menu_index++;
                       LCD_Clear();
                       LCD_DisplaySousMenu();
                       HAL_Delay(300);
                   }
               }
        }
    }
}

void LCD_DisplayMenu() {
	LCD_PrintFormatted((char *)menu_principal[menu_index]);  // Première ligne
    LCD_SendCommand(0xC0); // Passer à la ligne suivante
    LCD_PrintFormatted((char *)menu_principal[menu_index + 1]); // Deuxième ligne
}

void LCD_DisplaySousMenu() {

	 if (menu_index == 0 && sous_menu_index < sizeof(menu_Musics) / sizeof(menu_Musics[0])) {
	        LCD_PrintFormatted((char *)menu_Musics[sous_menu_index]);
	        LCD_SendCommand(0xC0);
	        if (sous_menu_index + 1 < sizeof(menu_Musics) / sizeof(menu_Musics[0])) {
	            LCD_PrintFormatted((char *)menu_Musics[sous_menu_index + 1]);
	        }
	}

 else if (menu_index == 1 && sous_menu_index < sizeof(menu_BPM) / sizeof(menu_BPM[0])) {
        LCD_PrintFormatted((char *)menu_BPM[sous_menu_index]);
        LCD_SendCommand(0xC0);
        if (sous_menu_index + 1 < sizeof(menu_BPM) / sizeof(menu_BPM[0])) {
            LCD_PrintFormatted((char *)menu_BPM[sous_menu_index + 1]);
        }
    } else if (menu_index == 2 && sous_menu_index < sizeof(menu_Color) / sizeof(menu_Color[0])) {
        LCD_PrintFormatted((char *)menu_Color[sous_menu_index]);
        LCD_SendCommand(0xC0);
        if (sous_menu_index + 1 < sizeof(menu_Color) / sizeof(menu_Color[0])) {
            LCD_PrintFormatted((char *)menu_Color[sous_menu_index + 1]);
        }
    }
}
