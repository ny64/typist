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
                    printf("No time specified.\n\n");
                    print_help();
                    exit(1);
                } else if (atoi(arg) < 1) {
                    printf("Time provided must be a positive integer.\n\n");
                    print_help();
                    exit(1);
                }
                tt.time = (unsigned int)atoi(arg);
                break;
            case 'r':
                if (arg[0] == '-' && !atoi(arg)) {
                    printf("Amount of words not specified.\n\n");
                    print_help();
                    exit(1);
                } else if (atoi(arg) < 1) {
                    printf("Amount of words must be a positive integer.\n\n");
                    print_help();
                    exit(1);
                }
                tt.random = (unsigned int)atoi(arg);
                break;
        }
    }

    if (arg[0] == '-') {
        if (!strcmp(arg, "-t") || !strcmp(arg, "--time")) {
            curarg = 't';
        } else if (!strcmp(arg, "-r") || !strcmp(arg, "--random")) {
            curarg = 'r';
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

