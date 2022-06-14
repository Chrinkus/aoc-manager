#include <stdlib.h>
#include <stdio.h>

#include <getopt.h>

const char* short_opts = "vh";
const struct option long_opts[] = {
        { .name = "version", .has_arg = no_argument, .flag = 0, .val = 'v' },
        { .name = "help", .has_arg = no_argument, .flag = 0, .val = 'h' },
        { .name = 0, .has_arg = 0, .flag = 0, .val = 0 },
};

int main(int argc, char* argv[])
{
        while (1) {
                int index = 0;
                int c = getopt_long(argc, argv, short_opts, long_opts, &index);

                if (c == -1)
                        break;

                switch (c) {
                case 0:
                        printf("Option with nothing to do..?\n");
                        break;
                case 'v':
                        printf("Version information\n");
                        break;
                case 'h':
                        printf("Help message\n");
                        break;
                default:
                        printf("How did we even get here..?\n");
                        break;
                }
        }

        exit(EXIT_SUCCESS);
}

