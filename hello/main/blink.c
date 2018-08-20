/*
 * 	blink.c
 */

#include "driver/gpio.h"

#include "blink.h"
#include "tdelay.h"

static void
led_off( void )
{
	gpio_set_level(BLINK_GPIO, 0);
}

static void
led_on( void )
{
	gpio_set_level(BLINK_GPIO, 1);
}


void
blinky(void *pvParameter)
{
 
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1)
	{
		led_off();
        tdelay(1000);
	
		led_on();
		tdelay(1000);
    }
}
 

