/*
 * midi_parser.h
 *
 *  Created on: Oct 8, 2024
 *      Author: ASUS
 */

#ifndef INC_MIDI_PARSER_H_
#define INC_MIDI_PARSER_H_

#include <stdint.h>


extern uint32_t tempo_us;
extern const uint8_t midi_data[];
extern const size_t midi_data_length;

int midi_note_to_led(uint8_t note);
void process_midi(uint8_t *midi_data, size_t length);

#endif /* INC_MIDI_PARSER_H_ */
