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
#include "run_command.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>

#include <fcntl.h>
#include <unistd.h>

enum {
        FIRST_AOC_DAY = 1,                      // Event runs from Dec 1st..
        LAST_AOC_DAY = 25,                      // ..to Dec 25th

        YEAR_LEN = 4,                                   // "2017"
        SEP_LEN = 1,                                    // '-'
        DAY_LEN = 2,                                    // "07"
        YEARDAY_LEN = YEAR_LEN + SEP_LEN + DAY_LEN,     // "2017-07"

        DOT_LEN = 1,                                    // '.'
        TXT_LEN = 3,                                    // "txt"
        EXT_LEN = DOT_LEN + TXT_LEN,                    // ".txt"
        INPUT_LEN = YEAR_LEN + EXT_LEN,                 // "2017-07.txt"

        DAY_BUFF = DAY_LEN + 1,
        YEAR_BUFF = YEAR_LEN + 1,
        INPUT_BUFF = INPUT_LEN + 1,
};

struct solution {
        char* exefile;
        char year[YEAR_BUFF];
        char day[DAY_BUFF];
        int flags;
};

// TODO: fetch these from local config file
const char* prefix = "/home/chrinkus/Documents/aoc_inputs";
const char* extension = "txt";

const char* run_short_opts = "hy:d:";
const struct option run_long_opts[] = {
        {
                .name = "help",
                .has_arg = no_argument,
                .flag = 0,
                .val = 'h'
        },
        {
                .name = "year",
                .has_arg = required_argument,
                .flag = 0,
                .val = 'y'
        },
        {
                .name = "day",
                .has_arg = required_argument,
                .flag = 0,
                .val = 'd'
        },
        { .name = 0, .has_arg = 0, .flag = 0, .val = 0 }
};

static char*
get_input_prefix(void)
{
        return prefix;
}

static char*
compose_input_file(const struct solution* sol)
{
        const char* prefix = get_input_prefix();
        const size_t len = strlen(prefix);

        char* buffer = malloc(len + INPUT_BUFF);

        sprintf(buffer, "%s/%s-%s.%s", prefix, sol->year, sol->day, extension);
        
        return buffer;
}

static int
run_solution(struct solution sol)
{
        char* newargv[] = { sol.exefile, NULL };
        char* newenv[] = { NULL };

        if (close(STDIN_FILENO) == -1) {
                perror("close");
                exit(EXIT_FAILURE);
        }

        char* input = compose_input_file(&sol);
        if (!input)
                exit(EXIT_FAILURE);

        printf("Input: %s\n", input);

        int fdin = open(input, O_RDONLY);
        if (fdin == -1) {
                perror("open");
                exit(EXIT_FAILURE);
        }
        free(input);

        execve(sol.exefile, newargv, newenv);
        perror("execve");
        exit(EXIT_FAILURE);
}

static const char*
is_valid_aoc_year(const char* y)
{
        if (strlen(y) != YEAR_LEN)
                return NULL;

        if (y[0] != '2' || y[1] != '0' || !isdigit(y[2]) || !isdigit(y[3]))
                return NULL;

        return y;
}

static const char*
is_valid_aoc_day(const char* d)
{
        int len = strlen(d);
        if (len > DAY_LEN || (len == DAY_LEN && !isdigit(d[1])))
                return NULL;

        int day = atoi(d);
        if (day < FIRST_AOC_DAY || LAST_AOC_DAY < day)
                return NULL;
        return d;
}

int
run_command(int argc, char* argv[])
{
        struct solution sol = { .year[0] = '\0', .day[0] = '\0' };

        while (1) {
                int i = 0;
                int c = getopt_long(argc, argv, run_short_opts,
                                run_long_opts, &i);

                if (c == -1) 
                        break;

                switch (c) {
                case 'h':
                        printf("Run help\n");
                        return EXIT_SUCCESS;
                case 'y':
                        if (!is_valid_aoc_year(optarg)) {
                                fprintf(stderr, "Invalid year: %s\n", optarg);
                                return EXIT_FAILURE;
                        }
                        strcpy(sol.year, optarg);
                        break;
                case 'd':
                        if (!is_valid_aoc_day(optarg)) {
                                fprintf(stderr, "Invalid day: %s\n", optarg);
                                return EXIT_FAILURE;
                        }
                        strcpy(sol.day, optarg);
                        if (strlen(sol.day) == 1) {
                                sol.day[1] = sol.day[0];
                                sol.day[0] = '0';
                        }
                        break;
                default:
                        printf("Run command default case..\n");
                        break;
                }
        }

        sol.exefile = argv[optind];
        return run_solution(sol);
}

