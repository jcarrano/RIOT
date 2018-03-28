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

#include "msg.h"
#include "shell.h"

#include "main.lua.h"

extern int lua_main(int argc, char **argv);
extern int lua_handle_buffer(const char *buffer, size_t buffer_len);

int lua_run_main_script(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    return lua_handle_buffer(main_lua, main_lua_len);
}

static const shell_command_t shell_commands[] = {
    { "lua", "Start a Lua prompt", lua_main},
    { "run_lua_script", "Run main.lua", lua_run_main_script},
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("Lua RIOT build");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
