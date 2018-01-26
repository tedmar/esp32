/*
 * 	test_led.c
 */

#include "driver/gpio.h"

#include "test_led.h"

#define BLINK_GPIO 2

static unsigned state;

void
init_test_led( void )
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT); /* Set the GPIO as a push/pull output */	
	led_off();
}

void
led_off( void )
{
	gpio_set_level(BLINK_GPIO, state = 0);
}

void
led_on( void )
{
	gpio_set_level(BLINK_GPIO, state = 1);
}

void
led_toggle( void )
{
	state ^= 1;
	gpio_set_level(BLINK_GPIO, state);
}


