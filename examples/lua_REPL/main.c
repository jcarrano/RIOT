/*
 * Copyright (C) 2018 FU Berlin
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
 * @brief       Lua shell in RIOT
 *
 * @author      Juan Carrano <j.carrano@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "lua_run.h"
#include "repl.lua.h"

#define MAIN_LUA_MEM_SIZE (20000)

char lua_memory[MAIN_LUA_MEM_SIZE] __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));

#define BARE_MINIMUM_MODS (LUAR_LOAD_BASE|LUAR_LOAD_IO|LUAR_LOAD_CORO)

int main(void)
{
    printf("%p - %p %u\n", lua_memory, lua_memory+MAIN_LUA_MEM_SIZE, sizeof(void*));

    while (1) {
        int status, value;
        puts("This is Lua: starting interactive session\n");

        status = luaR_do_buffer(repl_lua, repl_lua_len, lua_memory, MAIN_LUA_MEM_SIZE,
                                BARE_MINIMUM_MODS, &value);

        printf("Exited. status: %d, return code %d\n", status, value);
        break;
    }

    return 0;
}
