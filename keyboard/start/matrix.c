/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    DDRD |= (1<<6);
    PORTD |= (1<<6);

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13
 * pin: B2  B3  F4  B1  E6  B0  E7  F2  F7  F6  F5  F3  F0  F1
 */
static void  init_cols(void)
{
    // initialize columns to input with pull-up(DDR:0, PORT:1)
    
	/* E6 E7*/
    DDRE  &= ~(1<<7 | 1<<6);
    PORTE |=  (1<<7 | 1<<6);

    /* F7 ~ F0 */
    DDRF  &= ~(1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
    PORTF |=  (1<<7 | 1<<6 | 1<<5 | 1<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0);
	
	/* B3 B2 B1 B0 */
    DDRB  &= ~(1<<3 | 1<<2 | 1<<1 | 1<<0);
    PORTB |=  (1<<3 | 1<<2 | 1<<1 | 1<<0);
}

static matrix_row_t read_cols(void)
{
    return (PINB&(1<<2) ? 0 : (1<<0)) |
           (PINB&(1<<3) ? 0 : (1<<1)) |
           (PINF&(1<<4) ? 0 : (1<<2)) |
           (PINB&(1<<1) ? 0 : (1<<3)) |
           (PINE&(1<<6) ? 0 : (1<<4)) |
           (PINB&(1<<0) ? 0 : (1<<5)) |
           (PINE&(1<<7) ? 0 : (1<<6)) |
           (PINF&(1<<2) ? 0 : (1<<7)) |
           (PINF&(1<<7) ? 0 : (1<<8)) |
           (PINF&(1<<6) ? 0 : (1<<9)) |
           (PINF&(1<<5) ? 0 : (1<<10)) |
           (PINF&(1<<3) ? 0 : (1<<11)) |
           (PINF&(1<<0) ? 0 : (1<<12)) |
           (PINF&(1<<1) ? 0 : (1<<13)); 
}

/* Row pin configuration
 * row: 0   1   2   3   4   5   6   7
 * pin: C2  C3  C6  C5  C4  C1  C0  E1
 */
static void unselect_rows(void)
{
    // Output High (There is different from others!)
    DDRC = 0b11111111;
    PORTC = 0b11111111;
    DDRE |= (1<<1);
    PORTE |= (1<<1);
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRC  |= (1<<2);
            PORTC &= ~(1<<2);
            break;
        case 1:
            DDRC  |= (1<<3);
            PORTC &= ~(1<<3);
            break;
        case 2:
            DDRC  |= (1<<6);
            PORTC &= ~(1<<6);
            break;
        case 3:
            DDRC  |= (1<<5);
            PORTC &= ~(1<<5);
            break;
        case 4:
            DDRC  |= (1<<4);
            PORTC &= ~(1<<4);
            break;
        case 5:
            DDRC  |= (1<<1);
            PORTC &= ~(1<<1);
            break;
        case 6:
            DDRC  |= (1<<0);
            PORTC &= ~(1<<0);
            break;
        case 7:
            DDRE  |= (1<<1);
            PORTE &= ~(1<<1);
            break;
    }
}
