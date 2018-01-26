/*
 * 	htask.c
 */

#include <stdio.h>

#include "htask.h"
#include "tdelay.h"

void
hello_task(void *pvParameter)
{
	static int count;

	while(1)
	{
	    printf("Hello world again! %5d\n", count++);
	    tdelay(500);
	}
}

