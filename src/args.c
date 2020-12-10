#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "exit.h"
#include "output.h"

static char curarg;

int parse_argument(char *arg) {
    if (curarg != 0) {
        switch (curarg) {
            case 't':
                if (arg[0] == '-' && !atoi(arg)) {
                    die("No time specified.", 1);
                } else if (atoi(arg) < 1) {
                    die("Time provided must be a positive integer.", 1);
                }
                tt.time = (unsigned int)atoi(arg);
                break;
            case 'l':
                if (arg[0] == '-' && !atoi(arg)) {
                    die("No word length specified.", 1);
                } else if (atoi(arg) < 1) {
                    die("Average word length provided must be "\
                    "a positive floating point number.", 1);
                }
                tt.avg_word_length = (float)atoi(arg);
                break;
        }
    }

    if (arg[0] == '-') {
        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            print_help();
            exit(0);
        } else if (!strcmp(arg, "-t") || !strcmp(arg, "--time")) {
            curarg = 't';
        } else if (!strcmp(arg, "--average-word-length")) {
            curarg = 'l';
        } else {
            printf("Unknown argument %s.\n\n", arg);
            print_help();
            exit(1);
        }
        return 1;
    } else if (curarg == 0) {
        tt.filename = arg;
        return 0;
    }
    curarg = 0;

    return 1;
}
