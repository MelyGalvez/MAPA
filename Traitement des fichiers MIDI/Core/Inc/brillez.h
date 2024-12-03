/*
 * brillez.h
 *
 *  Created on: Nov 19, 2024
 *      Author: ASUS
 */

#ifndef INC_BRILLEZ_H_
#define INC_BRILLEZ_H_

#include <stdint.h>
#include "ws2812b.h"

extern TIM_HandleTypeDef htim1;
extern h_ws2812b_t h_ws2812b;

void brillez_init();
void blink(uint32_t color, uint32_t delay);
void light_up_led(uint16_t index, uint32_t color, uint32_t delay);

#endif /* INC_BRILLEZ_H_ */
