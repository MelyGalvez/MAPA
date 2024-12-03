/*
 * midi_parser.c
 *
 *  Created on: Oct 8, 2024
 *      Author: ASUS
 */


// midi_parser.c
#include "brillez.h"
#include "ws2812b.h"
#include "stm32l4xx_hal.h"
#include "midi_parser.h"

#define WS2812B_LED_NUMBER 30

extern h_ws2812b_t h_ws2812b;

uint32_t tempo_us = 1000000;  // Microseconds per quarter note (default 1s)
uint32_t ticks_per_beat = 96;  // Ticks per beat (could be 96 or 480 depending on the MIDI file)
uint32_t tick_duration_us;

// Tableau des commandes MIDI
const uint8_t midi_data[] = {
		 0x90, 0x3c, 0x5a, //Note ON on channel 1, note 60(0x3c), velocity 90(0x5a)
		 0x91, 0x24, 0x5a, //Note ON on channel 2, note 36(0x24), velocity 90(0x5a)
		 0x91, 0x28, 0x5a,
	     0x91, 0x2b, 0x5a,
		 0x90, 0x3c, 0x00,
		 0x90, 0x3c, 0x5a,
		 0x90, 0x3c, 0x00,
		 0x90, 0x3c, 0x5a,
		 0x90, 0x3c, 0x00,
		 0x90, 0x3e, 0x5a,
		 0x90, 0x3e, 0x00,
		 0x90, 0x40, 0x5a,
		 0x91, 0x24, 0x00,
		 0x91, 0x28, 0x00,
		 0x91, 0x2b, 0x00,
		 0x91, 0x2b, 0x5a,
		 0x91, 0x2f, 0x5a,
		 0x91, 0x32, 0x5a,
		 0x90, 0x40, 0x00,
		 0x90, 0x3e, 0x5a,
		 0x90, 0x3e, 0x00,
		 0x90, 0x3c, 0x5a,
		 0x91, 0x2b, 0x00,
		 0x91, 0x2f, 0x00,
		 0x91, 0x32, 0x00,
		 0x91, 0x29, 0x5a,
		 0x91, 0x2d, 0x5a,
		 0x91, 0x30, 0x5a,
		 0x90, 0x3c, 0x00,
		 0x90, 0x40, 0x5a,
		 0x90, 0x40, 0x00,
		 0x90, 0x3e, 0x5a,
		 0x91, 0x29, 0x00,
		 0x91, 0x2d, 0x00,
		 0x91, 0x30, 0x00,
		 0x91, 0x2b, 0x5a,
		 0x91, 0x2f, 0x5a,
		 0x91, 0x32, 0x5a,
		 0x90, 0x3e, 0x00,
		 0x90, 0x3e, 0x5a,
		 0x90, 0x3e, 0x00,
		 0x90, 0x3c, 0x5a,
		 0x91, 0x2b, 0x00,
		 0x91, 0x2f, 0x00,
		 0x91, 0x32, 0x00,
		 0x91, 0x30, 0x5a,
		 0x91, 0x34, 0x5a,
		 0x91, 0x37, 0x5a,
		 0x90, 0x3c, 0x00,
		 0x91, 0x30, 0x00,
		 0x91, 0x34, 0x00,
		 0x91, 0x37, 0x00
};
const size_t midi_data_length = sizeof(midi_data) / sizeof(midi_data[1]);



// Map MIDI note to LED index
int midi_note_to_led(uint8_t note) {
    if (note >= 31 && note < 61) {
        return note - 31;  // Map note range 31-60 to LED indices 0-29
    }
    return -1;  // Return -1 if the note is outside the range of available LEDs
}


void process_midi(uint8_t *midi_data, size_t length) {
    size_t i = 0;  // Index to parse MIDI data
    tick_duration_us = tempo_us / ticks_per_beat;

    while (i < length) {
        uint8_t status = midi_data[i++]; // MIDI status byte
        uint8_t note = midi_data[i++];   // MIDI note
        uint8_t velocity = midi_data[i++]; // MIDI velocity

        int led_index = midi_note_to_led(note); // Convert note to LED index

        if (led_index >= 0 && led_index < WS2812B_LED_NUMBER) {
            if ((status & 0xF0) == 0x90 && velocity > 0) { // Note On
                uint32_t color = (status == 0x90) ? 0xFF0000 : 0xFF0000; // Red for channel 1, Blue for channel 2
                ws2812b_set_led_hex(&h_ws2812b, led_index, color);
                ws2812b_send_buffer(&h_ws2812b);
            }
            if ((status & 0xF1) == 0x91 && velocity > 0) { // Note On
                uint32_t color = (status == 0x90) ? 0xFF0000 : 0x00FF00; // Red for channel 1, Blue for channel 2
                ws2812b_set_led_hex(&h_ws2812b, led_index, color);
                ws2812b_send_buffer(&h_ws2812b);
            }
            if ((status & 0xF0) == 0x80 || velocity == 0) { // Note Off
                ws2812b_set_led_hex(&h_ws2812b, led_index, 0x000000); // Turn off LED
                ws2812b_send_buffer(&h_ws2812b);
            }
        }
        HAL_Delay(tick_duration_us / 1000); // Delay in milliseconds
    }
}




