/*
* midi_parser.c
*
* Created on: Oct 8, 2024
* Author: ASUS
*/

/*
// midi_parser.c
#include <ws2812b.h>
#include "brillez.h"
#include "stm32l4xx_hal.h"
#include "midi_parser.h"

#define WS2812B_LED_NUMBER 144  // Total number of LEDs across all strips
#define NUM_ETAGES 2  // Number of LED strips (change as needed)

uint8_t active_notes[WS2812B_LED_NUMBER] = {0}; // 0 = éteint, 1 = allumé

extern h_ws2812b_t h_ws2812b;

uint32_t tempo_us = 750000; // 750000 µs for 80 BPM
uint32_t ticks_per_beat = 480; // Ticks per beat (could be 96 or 480 depending on the MIDI file)
uint32_t tick_duration_us;

// Determine the number of LEDs per floor
const int LEDS_PER_ETAGE = WS2812B_LED_NUMBER / NUM_ETAGES;

const uint16_t midi_data[] = {
    0x0000, 0xb0, 0x07, 0x64,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x40, 0x5a,
    0x0180, 0x90, 0x40, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x0180, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x40, 0x5a,
    0x00c0, 0x90, 0x40, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x0300, 0x90, 0x3c, 0x00,
    0x0000, 0xb1, 0x07, 0x64,
    0x0000, 0x91, 0x24, 0x5a,
    0x0030, 0x91, 0x28, 0x5a,
    0x0030, 0x91, 0x2b, 0x5a,
    0x02a0, 0x91, 0x24, 0x00,
    0x0000, 0x91, 0x28, 0x00,
    0x0000, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2b, 0x5a,
    0x0030, 0x91, 0x2f, 0x5a,
    0x0030, 0x91, 0x32, 0x5a,
    0x02a0, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2f, 0x00,
    0x0000, 0x91, 0x32, 0x00,
    0x0000, 0x91, 0x29, 0x5a,
    0x0030, 0x91, 0x2d, 0x5a,
    0x0030, 0x91, 0x30, 0x5a,
    0x0120, 0x91, 0x29, 0x00,
    0x0000, 0x91, 0x2d, 0x00,
    0x0000, 0x91, 0x30, 0x00,
    0x0000, 0x91, 0x2b, 0x5a,
    0x0030, 0x91, 0x2f, 0x5a,
    0x0030, 0x91, 0x32, 0x5a,
    0x0120, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2f, 0x00,
    0x0000, 0x91, 0x32, 0x00,
    0x0000, 0x91, 0x30, 0x5a,
    0x0030, 0x91, 0x34, 0x5a,
    0x0030, 0x91, 0x37, 0x5a,
    0x02a0, 0x91, 0x30, 0x00,
    0x0000, 0x91, 0x34, 0x00,
    0x0000, 0x91, 0x37, 0x00,
};
const size_t midi_data_length = sizeof(midi_data) / sizeof(midi_data[1]);


// Map MIDI note to LED index
int midi_note_to_led(uint8_t note) {
	if (note >= 21 && note < 109) {
		return note - 21; // Map MIDI note range (21-108) to LED index (0-87)
	}
	return -1;
}

// Met à jour les LEDs en fonction des notes actives
void update_leds() {
	for (int i = 0; i < WS2812B_LED_NUMBER; i++) {
		if (active_notes[i]) {
			ws2812b_set_led_hex(&h_ws2812b, i, 0xFF0000); // Rouge
		} else {
			ws2812b_set_led_hex(&h_ws2812b, i, 0x000000); // Éteint
		}
	}
	ws2812b_send_buffer(&h_ws2812b);
}

// Joue la séquence MIDI
void process_midi(uint16_t *midi_data, size_t length) {
    size_t i = 0;
    uint32_t current_time = 0;
    tick_duration_us = tempo_us / ticks_per_beat;  // Calculate duration of a tick
    uint32_t start_time = HAL_GetTick();
    uint32_t delay_offset_ms = 500; // Delay between étages

    while (i < length) {
        uint16_t delta_time = midi_data[i++]; // Time before this event
        uint16_t status = midi_data[i++]; // MIDI event type
        uint16_t note = midi_data[i++]; // MIDI note
        uint16_t velocity = midi_data[i++]; // Velocity

        current_time += (delta_time * tick_duration_us) / 1000; // Convert to ms

        // Wait until we reach the correct moment to execute this event
        while (HAL_GetTick() < start_time + current_time) {
            // Busy wait to maintain accurate timing
        }

        int led_index = midi_note_to_led(note); // Get the LED index from MIDI note
        int etage = led_index % NUM_ETAGES; // Assign the note to a floor

        if (led_index >= 0 && led_index < WS2812B_LED_NUMBER) {
            int led_index_etage = (etage * LEDS_PER_ETAGE) + (led_index / NUM_ETAGES);

            // Light up the LED on the correct floor first
            if ((status & 0xF0) == 0x90 && velocity > 0) { // Note On
                active_notes[led_index_etage] = 1;
            } else if ((status & 0xF0) == 0x80 || velocity == 0) { // Note Off
                active_notes[led_index_etage] = 0;
            }

            update_leds();

            // Delay before propagating to the next floor
            HAL_Delay(delay_offset_ms);

            // Propagate note to the lower floors
            for (int j = etage + 1; j < NUM_ETAGES; j++) {
                int lower_led_index = (j * LEDS_PER_ETAGE) + (led_index / NUM_ETAGES);

                if (((status & 0xF0) == 0x90 || ((status & 0xF1) == 0x91)) && velocity > 0) {
                    active_notes[lower_led_index] = 1;
                } else if ((status & 0xF0) == 0x80 || velocity == 0) {
                    active_notes[lower_led_index] = 0;
                }

                update_leds();
                HAL_Delay(delay_offset_ms); // Delay between floors
            }
        }
    }
}


// Joue la séquence MIDI
void process_midi(uint16_t *midi_data, size_t length) {
    size_t i = 0;
    uint32_t current_time = 0;
    tick_duration_us = tempo_us / ticks_per_beat; // Durée d'un tick en µs
    uint32_t start_time = HAL_GetTick();
    uint32_t delay_offset_ms = 500; // Décalage temporel entre les segments

    while (i < length) {
        uint16_t delta_time = midi_data[i++]; // Temps avant cet événement
        uint16_t status = midi_data[i++]; // Type d'événement MIDI
        uint16_t note = midi_data[i++]; // Note MIDI
        uint16_t velocity = midi_data[i++]; // Vélocité

        current_time += (delta_time * tick_duration_us) / 1000; // Conversion en ms

        // Attendre jusqu'à atteindre le bon moment
        while (HAL_GetTick() < start_time + current_time) {
            // Attente active pour synchronisation précise
        }

        // Calculer l'index dans le deuxième segment (72-143)
        int led_index_2 = midi_note_to_led(note);

        if (led_index_2 >= 0 && led_index_2 < 72) {
        	int led_index_1 = 71 - led_index_2; // Correspondance inversée avec le 1er segment

            // 🎹 Allumage du deuxième segment (72-143) EN PREMIER
            if (((status & 0xF0) == 0x90 || (status & 0xFF) == 0x91) && velocity > 0) { // Note On
             	active_notes[led_index_2 + 72] = 1;
            } else if ((status & 0xF0) == 0x80 || velocity == 0) { // Note Off
                active_notes[led_index_2 + 72] = 0;
            }

            update_leds();

            // ⏳ Attendre le délai AVANT d'allumer le premier segment
            HAL_Delay(delay_offset_ms);

            // 🎹 Ensuite, reproduire sur le premier segment (0-71) avec inversion
            if (((status & 0xF0) == 0x90 || (status & 0xFF) == 0x91) && velocity > 0) {
                active_notes[led_index_1] = 1;
            } else if ((status & 0xF0) == 0x80 || velocity == 0) {
                active_notes[led_index_1] = 0;
            }

            update_leds();
        }
    }
}



void process_midi(uint8_t *midi_data, size_t length) {
size_t i = 0;
tick_duration_us = tempo_us / ticks_per_beat; // Compute tick duration

while (i < length) {
uint8_t delta_time = midi_data[i++]; // Read delay before playing note
uint8_t status = midi_data[i++]; // MIDI event type
uint8_t note = midi_data[i++]; // MIDI note number
uint8_t velocity = midi_data[i++]; // Note intensity

int led_index = midi_note_to_led(note); // Convert note to LED index

// Delay before playing this note
HAL_Delay(delta_time * tick_duration_us / 1000);

if (led_index >= 0 && led_index < WS2812B_LED_NUMBER) {
if ((status & 0xF0) == 0x90 && velocity > 0) { // Note On
ws2812b_set_led_hex(&h_ws2812b, led_index, 0xFF0000); // Red LED
ws2812b_send_buffer(&h_ws2812b);
}
else if ((status & 0xF0) == 0x80 || velocity == 0) { // Note Off
ws2812b_set_led_hex(&h_ws2812b, led_index, 0x000000); // Turn off LED
ws2812b_send_buffer(&h_ws2812b);
}
}
}
}
*/


/*
 * midi_parser.c
 *
 *  Created on: Oct 8, 2024
 *      Author: ASUS
 */

#include "ws2812b.h"
#include "brillez.h"
#include "stm32l4xx_hal.h"
#include "midi_parser.h"

#define NUM_ETAGES 2  // Number of LED floors
#define WS2812B_LED_NUMBER 144/2 * NUM_ETAGES  // Total number of LEDs

#define LEDS_PER_ETAGE (WS2812B_LED_NUMBER / NUM_ETAGES) // LEDs per floor

uint8_t active_notes[WS2812B_LED_NUMBER] = {0}; // LED state array

extern h_ws2812b_t h_ws2812b;

uint32_t tempo_us = 500000; // 750000 µs for 80 BPM
uint32_t ticks_per_beat = 480; // Ticks per beat (can be 96 or 480)
uint32_t tick_duration_us;

const uint16_t midi_data[] = {
    0x0000, 0xb0, 0x07, 0x64,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x40, 0x5a,
    0x0180, 0x90, 0x40, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x0180, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x00c0, 0x90, 0x3c, 0x00,
    0x0000, 0x90, 0x40, 0x5a,
    0x00c0, 0x90, 0x40, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3e, 0x5a,
    0x00c0, 0x90, 0x3e, 0x00,
    0x0000, 0x90, 0x3c, 0x5a,
    0x0300, 0x90, 0x3c, 0x00,
    0x0000, 0xb1, 0x07, 0x64,
    0x0000, 0x91, 0x24, 0x5a,
    0x0030, 0x91, 0x28, 0x5a,
    0x0030, 0x91, 0x2b, 0x5a,
    0x02a0, 0x91, 0x24, 0x00,
    0x0000, 0x91, 0x28, 0x00,
    0x0000, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2b, 0x5a,
    0x0030, 0x91, 0x2f, 0x5a,
    0x0030, 0x91, 0x32, 0x5a,
    0x02a0, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2f, 0x00,
    0x0000, 0x91, 0x32, 0x00,
    0x0000, 0x91, 0x29, 0x5a,
    0x0030, 0x91, 0x2d, 0x5a,
    0x0030, 0x91, 0x30, 0x5a,
    0x0120, 0x91, 0x29, 0x00,
    0x0000, 0x91, 0x2d, 0x00,
    0x0000, 0x91, 0x30, 0x00,
    0x0000, 0x91, 0x2b, 0x5a,
    0x0030, 0x91, 0x2f, 0x5a,
    0x0030, 0x91, 0x32, 0x5a,
    0x0120, 0x91, 0x2b, 0x00,
    0x0000, 0x91, 0x2f, 0x00,
    0x0000, 0x91, 0x32, 0x00,
    0x0000, 0x91, 0x30, 0x5a,
    0x0030, 0x91, 0x34, 0x5a,
    0x0030, 0x91, 0x37, 0x5a,
    0x02a0, 0x91, 0x30, 0x00,
    0x0000, 0x91, 0x34, 0x00,
    0x0000, 0x91, 0x37, 0x00,
};

const size_t midi_data_length = sizeof(midi_data) / sizeof(midi_data[0]);

/**
 * Convert MIDI note to LED index.
 */
int midi_note_to_led(uint8_t note) {
    if (note >= 21 && note < 109) {
        return note - 21;  // Map MIDI note range (21-108) to LED range (0-87)
    }
    return -1;
}

/**
 * Converts LED index to the correct position in a serpentine layout.
 * Ensures symmetry at the boundary between floors.
 */
int serpentine_led_index(int led_index) {
    int etage = led_index / LEDS_PER_ETAGE;  // Determine the floor
    int pos_in_etage = led_index % LEDS_PER_ETAGE;  // Position in the floor

    if (etage % 2 == 1) {
        // Odd-numbered floors **reverse order**
        return ((etage + 1) * LEDS_PER_ETAGE) - 1 - pos_in_etage;
    } else {
        // Even-numbered floors **count forward**
        return (etage * LEDS_PER_ETAGE) + pos_in_etage;
    }
}

/**
 * Update the LED states.
 */
void update_leds() {
    for (int i = 0; i < WS2812B_LED_NUMBER; i++) {
        int real_index = serpentine_led_index(i); // Convert to correct order

        if (active_notes[i]) {
            ws2812b_set_led_hex(&h_ws2812b, real_index, 0xFF0000); // Red LED for active notes
        } else {
            ws2812b_set_led_hex(&h_ws2812b, real_index, 0x000000); // Turn off LED
        }
    }
    ws2812b_send_buffer(&h_ws2812b);
}

/**
 * Process MIDI data and control LEDs across serpentine-wired LED floors.
 * The highest étage lights up first, then moves downward.
 */
void process_midi(uint16_t *midi_data, size_t length) {
    size_t i = 0;
    uint32_t current_time = 0;
    tick_duration_us = tempo_us / ticks_per_beat; // Calculate tick duration
    uint32_t start_time = HAL_GetTick();
    uint32_t delay_offset_ms = 300; // Delay between floors

    while (i < length) {
        uint16_t delta_time = midi_data[i++]; // Read delay
        uint16_t status = midi_data[i++]; // MIDI event type
        uint16_t note = midi_data[i++]; // MIDI note
        uint16_t velocity = midi_data[i++]; // Velocity

        current_time += (delta_time * tick_duration_us) / 1000; // Convert to ms

        // Wait until the correct moment
        while (HAL_GetTick() < start_time + current_time) {}

        int led_index = midi_note_to_led(note);
        int etage = NUM_ETAGES - 1;  // Start from the highest étage

        if (led_index >= 0 && led_index < LEDS_PER_ETAGE) {
            // 🟢 Move note downward through the floors
            for (int j = etage; j >= 0; j--) {
                // Effect miroir : intervertir l'index entre les étages
                int mirrored_index = ((j == etage) ? led_index : (LEDS_PER_ETAGE - 1 - led_index));

                int lower_led_index = serpentine_led_index((j * LEDS_PER_ETAGE) + mirrored_index);

                if (((status & 0xF0) == 0x90 || (status & 0xFF) == 0x91) && velocity > 0) {
                    active_notes[lower_led_index] = 1;
                } else if ((status & 0xF0) == 0x80 || velocity == 0) {
                    active_notes[lower_led_index] = 0;
                }

                update_leds();
                HAL_Delay(delay_offset_ms);
            }
        }
    }
}









