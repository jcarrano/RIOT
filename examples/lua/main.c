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
 * @brief       Basic lua example application
 *
 * @author      Daniel Petry <daniel.petry@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include "shell.h"

#include "main.lua.h"

extern int lua_run_char_array(const char *buffer, size_t buffer_len);

int main(void)
{
    puts("Lua RIOT build");

    lua_run_char_array(main_lua, main_lua_len);

    return 0;
}
