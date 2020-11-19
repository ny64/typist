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
                printf("%d\n", atoi(arg));
                if (arg[0] == '-' && !atoi(arg)) {
                    printf("No time specified.\n\n");
                    print_help();
                    exit(1);
                } else if (atoi(arg) < 10) {
                    printf("Time provided must be an integer greater than"\
                            " or equal to 10.\n\n");
                    print_help();
                    exit(1);
                }
                tt.time = (unsigned int)atoi(arg);
                break;
        }
    }

    if (arg[0] == '-') {
        if (!strcmp(arg, "-t") || !strcmp(arg, "--time")) {
            curarg = 't';
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

