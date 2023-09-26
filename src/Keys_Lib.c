/*
 * Keys_Lib.c
 *
 * Created: 03.03.2022 8:39:56
 *  Author: PavlovVG
 */ 
 
#include "Keys_Lib.h"

#define KEYS_DEBOUNCE_TIME_MS	50
#define KEYS_INCREMENT_TIME_MS	200

void init_keys(Keys_t * keys,
        read_keys_callback_t read_keys_function,
		void * context,
        keys_mode_t keys_mode,
		const uint8_t keys_timer_period_ms) {
    keys->context = context;
    keys->read_keys_callback = read_keys_function;
    keys->keys_mode = keys_mode;
    keys->keys_timer_period_ms = keys_timer_period_ms;
    keys->keys_processing_state = WAITING_FOR_KEYS_PRESS_STATE;
    keys->keys_state = 0;
    keys->keys_debounce_ticks = KEYS_DEBOUNCE_TIME_MS / keys->keys_timer_period_ms;
    keys->keys_debounce_counter = keys->keys_debounce_ticks;
    keys->keys_increment_ticks = KEYS_INCREMENT_TIME_MS / keys->keys_timer_period_ms;
    keys->keys_increment_counter = keys->keys_increment_ticks;
    keys->keys_increment_boost_counter = 0;
}

void keys_debounce(Keys_t * keys, uint8_t keys_state) {
	if (keys->keys_processing_state != WAITING_FOR_KEYS_DEBOUNCE_STATE) {
		return;
	}
	if (keys_state && --keys->keys_debounce_counter != 0) {
		return;
	}
	if (keys->keys_debounce_counter == 0 && keys_state == 0) {
		keys->keys_debounce_counter = keys->keys_debounce_ticks;
		keys->keys_processing_state = WAITING_FOR_KEYS_PRESS_STATE;
		return;
	}
	keys->keys_debounce_counter = keys->keys_debounce_ticks;
	keys->keys_state = keys_state;
	keys->keys_mode == KEYS_CLICK_MODE ? (keys->keys_processing_state = KEYS_CLICK_STATE) : (keys->keys_processing_state = KEYS_INCREMENT_STATE);
}

void keys_click_mode(Keys_t * keys, uint8_t keys_state) {
	if (keys->keys_processing_state != KEYS_CLICK_STATE) {
		return;
	}
	if (keys_state == 0) {
		keys->keys_processing_state = WAITING_FOR_KEYS_PRESS_STATE;
	}
}

void keys_icnrement_mode(Keys_t * keys, uint8_t keys_state) {
	if (keys->keys_processing_state != KEYS_INCREMENT_STATE) {
		return;
	}
	if (keys_state == 0) {
		keys->keys_increment_boost_counter = 0;
		keys->keys_increment_counter = keys->keys_increment_ticks;
		keys->keys_processing_state = WAITING_FOR_KEYS_PRESS_STATE;
		return;
	}
	if (keys_state && --keys->keys_increment_counter != 0) {
		return;
	}
	keys->keys_state = keys_state;
	keys->keys_increment_counter = keys->keys_increment_ticks;
	keys->keys_increment_counter -= keys->keys_increment_boost_counter;
	if (keys->keys_increment_boost_counter < 10) {
		keys->keys_increment_boost_counter += 2;
	} else if (keys->keys_increment_boost_counter < keys->keys_increment_ticks - 40) {
		keys->keys_increment_boost_counter += 10;
	}
}

void keys_processing(Keys_t * keys) {
	uint8_t keys_state = keys->read_keys_callback(keys->context);
	if (keys->keys_processing_state == WAITING_FOR_KEYS_PRESS_STATE && keys_state) {
		keys->keys_processing_state = WAITING_FOR_KEYS_DEBOUNCE_STATE;
	}
	if (keys->keys_processing_state == WAITING_FOR_KEYS_PRESS_STATE) {
		return;
	}
	keys_debounce(keys, keys_state);
	keys_click_mode(keys, keys_state);
	keys_icnrement_mode(keys, keys_state);
}

uint8_t key_is_pressed(Keys_t * keys, keys_list_t key) {
	if (keys->keys_state & key) {
		keys->keys_state &= ~key;
		return 1;
	}
	return 0;
}
