/*
 * 	tdelay.c
 * 		Task delay
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tdelay.h"


void
tdelay( unsigned ms )
{
	vTaskDelay( ms / portTICK_RATE_MS );
}

