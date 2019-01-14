/*
 * Copyright (C) 2013 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @file
 * @brief       shows how to set up own and use the system shell commands.
 *              By typing help in the serial console, all the supported commands
 *              are listed.
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Zakaria Kasmi <zkasmi@inf.fu-berlin.de>
 *
 */

#include <stdio.h>
#include <string.h>

#include "shell_commands.h"
#include "shell.h"

static int print_teststart(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    printf("[TEST_START]\n");

    return 0;
}

static int print_testend(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    printf("[TEST_END]\n");

    return 0;
}

static int print_echo(int argc, char **argv)
{
    for (int i = 0; i < argc; ++i) {
        printf("\"%s\"", argv[i]);
    }
    puts("");

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "start_test", "starts a test", print_teststart },
    { "end_test", "ends a test", print_testend },
    { "echo", "prints the input command", print_echo },
    { NULL, NULL, NULL }
};

char *fake_command[] = {"fake_command"};
char *real_command[] = {"echo", "one", "two", "three"};

int main(void)
{

    printf("test_shell.\n");


    /* define buffer to be used by the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* define own shell commands */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    puts("shell exited (1)");

    /* Restart the shell after the previous one exits, so that we can test
     * both ways to exit (ctrl-C and ctrl-D). */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    puts("shell exited (2)");

    printf("Shell call bad command 1=%d\n",
           shell_call(shell_commands, 1, fake_command)<0? 1:0);

    printf("Shell call good command 1=%d\n",
           shell_call(shell_commands, sizeof(real_command)/sizeof(*real_command),
                      real_command)>=0? 1:0);

    /* Start the shell again so that the node may be reset by a command */
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* or use only system shell commands */
    /*
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
    */

    return 0;
}
