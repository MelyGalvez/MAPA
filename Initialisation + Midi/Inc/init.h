/*
 * bouton.h
 *
 *  Created on: Feb 4, 2025
 *      Author: lijia
 */
#include <stdint.h>

#ifndef INC_INIT_H_
#define INC_INIT_H_

#define NB_Touche 12

extern volatile uint32_t compteur;
extern volatile int32_t compteur_sauvegarde[NB_Touche];
extern volatile uint32_t sauvegarde_index;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
int valeur_existe_deja(int32_t valeur);
void supprimer_valeur(uint32_t valeur);
void Test();
void assignement();
void merge(volatile int32_t arr[], int left, int mid, int right);
void mergeSort(volatile int32_t arr[], int left, int right);
#endif /* INC_BOUTON_H_ */
