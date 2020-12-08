#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exit.h"
#include "helper.h"
#include "output.h"

void die(const char *s, int help) {
    CLR_SCREEN;
    CRS_POS_TOP;

    if (errno) {
        perror(s);
    } else {
        printf("ERROR: ");
        printf("%s\n", s);
    }

    if (help) {
        printf("\n\n");
        print_help();
    }

    exit(1);
}
