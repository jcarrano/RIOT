/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include "xtimer.h"

/* use chars to test the alignment */
static char counter __attribute__((section(".backup.data"))) = 8;
static char counter_zero __attribute__((section(".backup.bss")));
static char counter_noinit __attribute__((section(".backup.noinit")));

int main(void)
{
    puts("Hello World!");

    printf("counter value, %d\n", counter);
    printf("counter_zero value, %d\n", counter_zero);
    printf("counter_noinit value, %d\n", counter_noinit);

    while (1) {
        printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
        printf("This board features a(n) %s MCU.\n", RIOT_MCU);
        printf("counter value, %d\n", counter++);
        printf("counter_zero value, %d\n", counter_zero++);
        printf("counter_noinit value, %d\n", counter_noinit++);
        xtimer_sleep(1);
    }

    return 0;
}
