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

#include <avr/io.h>
#include <util/delay.h>
#include "stdint.h"
#include "led.h"

/*
 * LED_UPDATE
 * 		D7
 * LED:  0  1  2  3
 *       E1 C0 C1 C2
*/
#define LED_UPDATE_CONFIG	(DDRD |= (1<<7))
#define LED_UPDATE_1		(PORTD |= (1<<7))
#define LED_UPDATE_0 		(PORTD &= ~(1<<7))

#define LED0_CONFIG		(DDRE |= (1<<1))
#define LED0_OFF		(PORTE |= (1<<1))
#define LED0_ON 		(PORTE &= ~(1<<1))

#define LED1_CONFIG		(DDRC |= (1<<0))
#define LED1_OFF		(PORTC |= (1<<0))
#define LED1_ON 		(PORTC &= ~(1<<0))

#define LED2_CONFIG		(DDRC |= (1<<1))
#define LED2_OFF		(PORTC |= (1<<1))
#define LED2_ON 		(PORTC &= ~(1<<1))

#define LED3_CONFIG		(DDRC |= (1<<2))
#define LED3_OFF		(PORTC |= (1<<2))
#define LED3_ON 		(PORTC &= ~(1<<2))

void led_set(uint8_t usb_led)
{
	LED_UPDATE_CONFIG;
	LED0_CONFIG;
	LED1_CONFIG;
	LED2_CONFIG;
	LED3_CONFIG;

	if(usb_led & (1<<USB_LED_CAPS_LOCK))
		LED2_ON;
	else
		LED2_OFF;

	if(usb_led & (1<<USB_LED_NUM_LOCK))
		LED0_ON;
	else
		LED0_OFF;

	if(usb_led & (1<<USB_LED_SCROLL_LOCK))
		LED3_ON;
	else
		LED3_OFF;

	// raising edge twice for 74ls174 clock pin
	LED_UPDATE_1;
	_delay_us(1);
	LED_UPDATE_0;
	_delay_us(1);
	LED_UPDATE_1;
	_delay_us(1);
	LED_UPDATE_0;

    // Output High (There is different from others!)
    DDRC = 0b11111111;
    PORTC = 0b11111111;
    DDRE |= (1<<1);
    PORTE |= (1<<1);
}
