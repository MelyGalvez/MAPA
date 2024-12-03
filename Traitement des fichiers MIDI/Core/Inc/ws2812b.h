/*
 * ws2812b.h
 *
 *  Created on: Nov 19, 2024
 *      Author: ASUS
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

#include "stm32l4xx_hal.h"

#define WS2812B_LED_NUMBER 30
#define WS2812B_DATA_LENGTH WS2812B_LED_NUMBER * 24 + 1

typedef struct h_ws2812b_struct
{
	TIM_HandleTypeDef * htim;
	uint32_t tim_channel;
	uint16_t data[WS2812B_DATA_LENGTH];
} h_ws2812b_t;

int ws2812b_init(h_ws2812b_t * h_ws2812b);
int ws2812b_set_led_hex(h_ws2812b_t * h_ws2812b, uint16_t index, uint32_t hex);
int ws2812b_set_led_rgb(h_ws2812b_t * h_ws2812b, uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
int ws2812b_send_buffer(h_ws2812b_t * h_ws2812b);

#endif /* INC_WS2812B_H_ */
