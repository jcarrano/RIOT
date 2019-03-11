/*
 * Copyright (C) 2019 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief   Test malloc and free implementations.
 *
 * @author   Juan I Carrano <jcarrano@fu-berlin.de>
 *
 * This application presents several shell commands to interface to the malloc,
 * realloc and free functions. The test script can keep track of the memory
 * operations and use that to verify the malloc implementation.
 *
 * @}
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "shell_commands.h"
#include "shell.h"

/**
 * Wrapper around strtoul() with base=0.
 *
 * @param[out]  val     stores the result of the strtoul() conversion. Should
 *                      NOT be NULL.
 *
 * @return  0   if the entirety of the input string was successfully converted.
 * @return  nonzero     if part of the string was not converted.
 */
static int _strtoz(char *str, size_t *val)
{
    char *end_char;

    *val = strtoul(str, &end_char, 0);

    return *end_char != '\0';
}

/**
 * Like _strtoz but this one parses a pointer
 */
static int _strtop(char *str, intptr_t *val)
{
    char *end_char;

    *val = strtoul(str, &end_char, 0);

    return *end_char != '\0';
}

/**
 * Like _strtoul but this does not complain if it is given a pointer to int.
 */
static int _strtoi(char *str, int *val)
{
    char *end_char;

    *val = strtol(str, &end_char, 0);

    return *end_char != '\0';
}

static const char result_ok[] = "{ 'result' : 'SUCCESS' }";
static const char result_fail[] = "{ 'result' : 'ERROR' }";

static int cmd_malloc(int argc, char *argv[])
{
    size_t requested_size;
    void *result;

    if (argc != 2 || _strtoz(argv[1], &requested_size)) {
        puts("Usage: malloc <alloc_size>");
        puts("Allocate a block of size <alloc_size>. <alloc_size> can be decimal or hex");
        puts("Output: {'action': 'malloc', 'addr':<result of malloc>, 'size':<requested_size>'}");
        puts(result_fail);
        return 1;
    }

    result = malloc(requested_size);
    printf("{'action': 'malloc',\n 'addr': %p,\n 'size': %d}\n",
            result, requested_size);

    puts(result_ok);

    return 0;
}

/**
 * Parse a command line consisting of three integers.
 *
 * @return nonnegative on success, negative value on error.
 */
static int _parse_fill_args(int argc, char *argv[],
                            intptr_t *block_address,
                            size_t *block_size,
                            int *block_contents)
{
    if (argc != 4
        || _strtop(argv[1], block_address)
        || _strtoz(argv[2], block_size)
        || _strtoi(argv[3], block_contents)
        ) {
        printf("Usage: %s <address> <size> <value>\n", argv[0]);
        return -1;
    }

    return 0;
}

static int cmd_fill(int argc, char *argv[])
{
    intptr_t block_address;
    size_t block_size;
    int block_contents;

    if (_parse_fill_args(argc, argv,
                         &block_address, &block_size, &block_contents) < 0) {
        puts("Set every byte of the block of memory of size <size>");
        puts("starting at <address> to <value>");
        puts(result_fail);
        return 1;
    }

    memset((void*)block_address, block_contents, block_size);

    printf("{'action': 'fill',\n 'addr': %p,\n 'size': %d,\n 'value'=%u}\n",
         (void*)block_address, block_size, block_contents);
    puts(result_ok);

    return 0;
}

static int cmd_check(int argc, char *argv[])
{
    intptr_t block_address;
    size_t block_size;
    int block_contents;
    unsigned int i;
    char *block;
    bool check_ok = 1;

    if (_parse_fill_args(argc, argv,
                         &block_address, &block_size, &block_contents) < 0) {
        puts("Verify that every byte of the block of memory of size <size>");
        puts("starting at <address> has a value equal to <value>");
        puts("Output: 'result: 1' if successful, 'result: 0' otherwise.");
        puts(result_fail);
        return 1;
    }

    block = (char *)block_address;

    for (i = 0; i < block_size; i++) {
        if (block[i] != block_contents) {
            check_ok = 0;
            break;
        }
    }

    printf("{'action': 'check',\n 'addr': %p,\n 'size': %d,\n 'value'=%u\n, 'result': %d}\n",
         (void*)block_address, block_size, block_contents, check_ok);
    puts(result_ok);

    return 0;
}

static int cmd_free(int argc, char *argv[])
{
    intptr_t block_address;

    if (argc != 2
        || _strtop(argv[1], &block_address)
        ) {
        puts("Usage: free <address>");
        puts("Free the block located at <address>. <address> can be decimal or hex");
        return 1;
    }

    free((void *)block_address);

    printf("{'action': 'free',\n 'addr': %p}\n", (void *)block_address);

    puts(result_ok);

    return 0;
}

static int cmd_realloc(int argc, char *argv[])
{
    intptr_t block_address;
    size_t new_size;
    void *new_address;

    if (argc != 3
        || _strtop(argv[1], &block_address)
        || _strtoz(argv[2], &new_size)
        ) {
        puts("Usage: realloc <old_address> <new_size>");
        puts("Reallocate the block located at <old_address> so that the new size");
        puts("is <new_size>. The reallocation can result in the block being moved.");
        return 1;
    }

    new_address = realloc((void *)block_address, new_size);
    printf("{'action': 'realloc',\n 'addr': %p,\n 'new_addr': %p,\n 'new_size': %u}\n",
         (void *)block_address, new_address, new_size);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "malloc", "Allocate a block", cmd_malloc },
    { "fill", "Fill memory block with value", cmd_fill },
    { "check", "Check block contents", cmd_check },
    { "free", "Free block", cmd_free },
    { "realloc", "Reallocate", cmd_realloc },
    { NULL, NULL, NULL }
};

int main(void)
{
    static char line_buf[SHELL_DEFAULT_BUFSIZE];

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
