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
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include "support_dispatch.h"
#include "config.h"

const char* support_short_opts = "vh";
const struct option support_long_opts[] = {
        { .name = "version", .has_arg = no_argument, .flag = 0, .val = 'v' },
        { .name = "help", .has_arg = no_argument, .flag = 0, .val = 'h' },
        { .name = 0, .has_arg = 0, .flag = 0, .val = 0 },
};

static void
print_version(void)
{
        printf("aocm v%s\n", AOCM_VERSION_STRING);
}

static void
print_help(void)
{
        printf("The most helpful help ever\n");
}

static void
print_error(const char* prog)
{
        perror(prog);
        exit(1);
}

int
support_dispatch(int argc, char* argv[])
{
        while (1) {
                int index = 0;
                int c = getopt_long(argc, argv, support_short_opts,
                                support_long_opts, &index);

                if (c == -1)
                        break;

                switch (c) {
                case 'v':       print_version();        break;
                case 'h':       print_help();           break;
                default:        print_error(argv[0]);   break;
                }
        }
        return EXIT_SUCCESS;
}

void
print_missing_args(const char* prog)
{
        printf("%s: missing arguments\n"
               "Try '%s --help' for more information.\n",
               prog, prog);
}

