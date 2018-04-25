/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/******************************************************************************
* Copyright (C) 1994-2017 Lua.org, PUC-Rio.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

/**
 * @ingroup     pkg
 * @{
 *
 * @file        lua_run_char_array.c
 * @brief       Implementation of lua_run_char_array,  which runs a char array
 *              as a lua script.
 *
 * @author      Daniel Petry <daniel.petry@fu-berlin.de>
 *
 * @}
 */

#include <stdlib.h>
#include "lauxlib.h"
#include "lualib.h"

int lua_run_char_array (char *buffer, size_t buffer_len ){

    lua_State *L = luaL_newstate();

    if (L == NULL) {
        puts("cannot create state: not enough memory");
        return EXIT_FAILURE;
    }

    luaL_openlibs(L);

    luaL_loadbuffer(L, buffer, buffer_len, "lua input script");
  
    if (lua_pcall(L, 0, 0, 0) != LUA_OK){
        puts("Lua script running failed");
        return EXIT_FAILURE;
    }

    lua_close(L);

    return EXIT_SUCCESS;
}
