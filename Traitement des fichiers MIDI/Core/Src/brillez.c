/*
 * brillez.c
 *
 *  Created on: Nov 19, 2024
 *      Author: ASUS
 */

#include "brillez.h"
#include "ws2812b.h"

h_ws2812b_t h_ws2812b = {
    .htim = &htim1,
    .tim_channel = TIM_CHANNEL_1
};

void brillez_init()
{
	ws2812b_init(&h_ws2812b);
}


void blink(uint32_t color, uint32_t delay)
{
	for (int i = 0 ; i < WS2812B_LED_NUMBER ; i++)
	{
		ws2812b_set_led_hex(&h_ws2812b, i, color);
	}
	ws2812b_send_buffer(&h_ws2812b);
	HAL_Delay(delay);

	for (int i = 0 ; i < WS2812B_LED_NUMBER ; i++)
	{
		ws2812b_set_led_hex(&h_ws2812b, i, 0);
	}
	ws2812b_send_buffer(&h_ws2812b);
	HAL_Delay(delay);
}

// Function to light up a specific LED with a given color
void light_up_led(uint16_t index, uint32_t color, uint32_t delay) {
    if (index < WS2812B_LED_NUMBER) {
        // Light up the specified LED with the given color
        ws2812b_set_led_hex(&h_ws2812b, index, color);
        ws2812b_send_buffer(&h_ws2812b);

        HAL_Delay(delay);

        // Turn off the LED after the delay
        ws2812b_set_led_hex(&h_ws2812b, index, 0x000000);
        ws2812b_send_buffer(&h_ws2812b);
    }
}
