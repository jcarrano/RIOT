/*
 * Copyright (C) 2018 Freie Universität Berlin
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
 * @brief       TLSF initialization tests.
 *
 * @author      Juan I Carrano <j.carrano@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <stdint.h>

#ifdef MODULE_TLSF_MALLOC
#include "tlsf.h"
#include "tlsf-malloc.h"
#endif /* MODULE_TLSF_MALLOC */


#ifdef BOARD_NATIVE

    /*
     * On native the linker dos not define the Heap area.
     * It is handled by the C library (it requests memory from the OS ??
     */
    #define HEAP_SIZE (0x800000)
    static uint8_t _heap[HEAP_SIZE];

    #define HEAP_START ((void*)_heap)
    #define HEAP_END ((void*)(_heap + HEAP_SIZE))

    /* This causes a function to be run before main. Seems to work on native,
     * although the correct thing would be to run even before the C library
     * is initialized (in case it uses malloc) */
    #define PREINIT_ATTR __attribute__((constructor))

#else /* If we are in embedded */

    /*
     * The linker script for embedded target defines _sheap and _eheap:
     *
     *  / * heap section * /
     *  . = ALIGN(4);
     *  _sheap = . ;
     *  _eheap = ORIGIN(ram) + LENGTH(ram);
     *
     */
    extern void *_sheap;
    extern void *_eheap;

    #define HEAP_START (&_sheap)
    #define HEAP_END (&_eheap)
    #define HEAP_SIZE ((uint8_t *)HEAP_END - (uint8_t *)HEAP_START)

    /* We will handle this differently, see below */
    #define PREINIT_ATTR

#endif /* BOARD_NATIVE */

#ifdef MODULE_TLSF_MALLOC

#define ROUND_DOWN4(x) (((x)/4)*4) /* Is this necessary??? */

PREINIT_ATTR void init_tlsf_malloc(void)
{
    tlsf_add_global_pool(HEAP_START, ROUND_DOWN4(HEAP_SIZE));
}

#ifndef BOARD_NATIVE

/* This is only for embedded (not native).
 * Newlib runs the functions in this array before it initializes itself.
 * It is defined as a weak symbol and no one else uses it, so we can override it
 */
void (* const init_tlsf_malloc_p) (void) __attribute__((section (".preinit_array"))) =
    init_tlsf_malloc;

#endif /* ! BOARD_NATIVE */

#endif /* MODULE_TLSF_MALLOC */

static void walker_texas_ranger(void* ptr, size_t size, int used, void* _accumulator)
{
	size_t *accumulator = _accumulator;
    *accumulator += size;
	printf("\t%p %s size: %u\n", ptr, used ? "used" : "free", (unsigned int)size);
}

int main(void)
{
    size_t accumulator = 0;

    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    printf("Using "
#ifdef MODULE_TLSF_MALLOC
        "TLSF-malloc"
#else
        "newlib malloc"
#endif
        "\n");

    printf("Heap starts at %p and end at %p\n", (void *)HEAP_START, (void *)HEAP_END);
    printf("Heap has %u bytes\n", HEAP_SIZE);

#ifdef MODULE_TLSF_MALLOC
    puts("TLSF heap report:");
    tlsf_walk_pool(tlsf_get_pool(_tlsf_get_global_control()), walker_texas_ranger, &accumulator);
    printf("Accordinf to TLSF heap has %u bytes\n", accumulator);
#endif

    return 0;
}
