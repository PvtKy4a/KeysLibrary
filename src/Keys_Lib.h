/*
 * Keys_Lib.h
 *
 * Created: 03.03.2022 8:40:09
 *  Author: PavlovVG
 */ 

#ifndef KEYS_LIB_H_
#define KEYS_LIB_H_

#include <stdint.h>

typedef enum {
	KEY1 = 0x1,
	KEY2 = 0x2,
	KEY3 = 0x4,
	KEY4 = 0x8,
	KEY5 = 0x10,
	KEY6 = 0x20,
	KEY7 = 0x40,
	KEY8 = 0x80
} keys_list_t;

typedef enum {
	KEYS_CLICK_MODE,
	KEYS_INCREMENT_MODE
} keys_mode_t;

typedef enum {
	WAITING_FOR_KEYS_PRESS_STATE,
	WAITING_FOR_KEYS_DEBOUNCE_STATE,
	KEYS_CLICK_STATE,
	KEYS_INCREMENT_STATE
} keys_processing_state_t;

typedef uint8_t (* read_keys_callback_t)(void *);

typedef struct {
    void * context;
    read_keys_callback_t read_keys_callback;
    keys_mode_t keys_mode;
    uint8_t keys_timer_period_ms;
    keys_processing_state_t keys_processing_state;
    uint8_t keys_state;
    uint8_t keys_debounce_ticks;
    uint8_t keys_debounce_counter;
    uint16_t keys_increment_ticks;
    uint16_t keys_increment_counter;
    uint16_t keys_increment_boost_counter;
} Keys_t;

void init_keys(Keys_t * keys,
        read_keys_callback_t read_keys_function,
        void * context,
        keys_mode_t keys_mode,
		const uint8_t keys_timer_period_ms);

void keys_processing(Keys_t * keys);

uint8_t key_is_pressed(Keys_t * keys, keys_list_t key);

#endif /* KEYS_LIB_H_ */
