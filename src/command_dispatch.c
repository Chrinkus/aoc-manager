/* aoc-manager - A command line utility for managing Advent of Code inputs and
 * the running of solutions.
 *
 * Copyright (C) 2022 Chris Schick
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "command_dispatch.h"
#include "run_command.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>

const char* command_short_opts = "h";
const struct option command_long_opts[] = {
        { .name = "help", .has_arg = no_argument, .flag = 0, .val = 'h' },
        { .name = 0, .has_arg = 0, .flag = 0, .val = 0 },
};

typedef int (*SubCmd)(int argc, char* argv[]);

struct sub_command {
        char* name;
        SubCmd func;
};

const struct sub_command commands[] = {
        { .name = "run", .func = run_command, },
};

const size_t num_commands = sizeof(commands) / sizeof(commands[0]);

int
command_dispatch(int argc, char* argv[])
{
        for (size_t i = 0; i < num_commands; ++i)
                if (strcmp(argv[0], commands[i].name) == 0)
                        return commands[i].func(argc, argv);
        while (1) {
                int index = 0;
                int c = getopt_long(argc, argv, command_short_opts,
                        command_long_opts, &index);

                if (c == -1)
                        break;

                switch (c) {
                case 'h':
                        printf("Help message\n");
                        break;
                default:
                        printf("How did we even get here..?\n");
                        break;
                }
        }

        if (optind < argc) {
                printf("Non-option ARGV-elements: ");
                while (optind < argc)
                        printf("%s ", argv[optind++]);
                printf("\n");
        }

        return EXIT_SUCCESS;
}

