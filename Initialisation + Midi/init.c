	/*
	 * bouton.c
	 *
	 *  Created on: Feb 4, 2025
	 *      Author: lijia
	 */

	#include "init.h"
	#include <stdio.h>
	#include "main.h"
	#include "ecran.h"
	#include "midi_parser.h"
	#include "brillez.h"


	volatile uint32_t compteur = 0;
	volatile int32_t compteur_sauvegarde[88] = {0};
	volatile uint32_t sauvegarde_index = 0;

	#define DEBOUNCE_TIME 50  // Temps en ms (anti-rebond)
	volatile uint32_t last_interrupt_time = 0;

	// Vérifie si la valeur existe déjà dans compteur_sauvegarde
	int valeur_existe_deja(int32_t valeur) {
		for (int i = 0; i < sauvegarde_index; i++) {
			if (compteur_sauvegarde[i] == valeur) {
				return 1;  // La valeur est déjà présente
			}
		}
		return 0;  // La valeur n'existe pas
	}


	void supprimer_valeur(uint32_t valeur) {
		for (uint32_t i = 0; i < sauvegarde_index; i++) {
			if (compteur_sauvegarde[i] == valeur) {  // Si on trouve la valeur
				// Décaler les éléments restants pour combler l'espace
				for (uint32_t j = i; j < sauvegarde_index - 1; j++) {
					compteur_sauvegarde[j] = compteur_sauvegarde[j + 1];
				}
				sauvegarde_index--;  // Réduire la taille du tableau
				break;  // Quitter la boucle dès qu'on a supprimé la valeur
			}
		}
	}

	volatile uint8_t current_mode = 0;

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {



		if (GPIO_Pin == GPIO_PIN_13 && compteur < 144) {

			compteur++;  // Incrémentation


		} else if (GPIO_Pin == GPIO_PIN_0 && compteur > 0) {
			compteur--;  // Décrémentation
		} else if (GPIO_Pin == GPIO_PIN_9) {  // Bouton pressé(PA9)
			supprimer_valeur(compteur);
		} else if (GPIO_Pin == GPIO_PIN_1) {  // Sauvegarde du compteur
			if (!valeur_existe_deja(compteur)) {  // Vérifie si la valeur est déjà sauvegardée
				if (sauvegarde_index < 88) {
					compteur_sauvegarde[sauvegarde_index] = compteur;
					sauvegarde_index++;  // Incrémente l'index de sauvegarde
					compteur++;  // Incrémente le compteur après la sauvegarde
				} else {
					printf("Tableau de sauvegarde plein !\r\n");
					LCD_SendCommand(0xC0);
					LCD_Print("Tableau plein");

				}
			} else {
				printf("Valeur %ld déjà enregistrée, non ajoutée.\r\n", compteur);
				LCD_SendCommand(0xC0);
				LCD_PrintFormatted("Led %ld deja enregistré", compteur);

			}
		}

		if (sauvegarde_index == 8) {
				  for (uint32_t i = 0; i < sauvegarde_index; i++){
					  light_up_led(compteur_sauvegarde[i], 0xFF0000, 100);
				  }
				  process_midi((uint16_t *) midi_data, midi_data_length);
				  current_mode++;
			  }
		}*/



void Test(){
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {  // Bouton Entrer
	            LCD_Clear();
	            LCD_Print("PC13");}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {  // Bouton Entrer
		            LCD_Clear();
		            LCD_Print("PA1");}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET) {  // Bouton Entrer
		            LCD_Clear();
		            LCD_Print("PA9");}
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {  // Bouton Entrer
		            LCD_Clear();
		            LCD_Print("PA0");}

}
void assignement(){
	while(1){
		HAL_Delay(50);

		LCD_Clear();  // Efface l’écran avant d’écrire

		LCD_PrintFormatted("LED numero %lu", compteur);  // Affiche le compteur
		HAL_Delay(200);

		printf("Liste des sauvegardes : ");
		for (uint32_t i = 0; i < sauvegarde_index; i++) {
		  printf("%ld ", compteur_sauvegarde[i]);
		}
		printf("\r\n");

		light_up_led(compteur, 0xFF0000, 100);

		for (uint32_t i = 0; i < sauvegarde_index; i++) {
		  ws2812b_set_led_hex(&h_ws2812b,compteur_sauvegarde[i], 0x00FF00);
		}
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET && compteur < 144) {

			compteur++;  // Incrémentation


		} else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET && compteur > 0) {
			compteur--;  // Décrémentation
		} else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET) {  // Bouton pressé(PA9)
			supprimer_valeur(compteur);
		} else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {  // Sauvegarde du compteur
			if (!valeur_existe_deja(compteur)) {  // Vérifie si la valeur est déjà sauvegardée
				if (sauvegarde_index < 88) {
					compteur_sauvegarde[sauvegarde_index] = compteur;
					sauvegarde_index++;  // Incrémente l'index de sauvegarde
					compteur++;  // Incrémente le compteur après la sauvegarde
				} else {
					printf("Tableau de sauvegarde plein !\r\n");
					LCD_SendCommand(0xC0);
					LCD_Print("Tableau plein");

				}
			} else {
				printf("Valeur %ld déjà enregistrée, non ajoutée.\r\n", compteur);
				LCD_SendCommand(0xC0);
				LCD_PrintFormatted("Led %ld deja enregistré", compteur);

			}
		}

		if (sauvegarde_index == 8) {
				  for (uint32_t i = 0; i < sauvegarde_index; i++){
					  light_up_led(compteur_sauvegarde[i], 0xFF0000, 100);
				  }
				  process_midi((uint16_t *) midi_data, midi_data_length);
				  break;
			  }
	}
}
