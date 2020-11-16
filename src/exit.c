#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exit.h"
#include "helper.h"

void die(const char *s) {
    CLR_SCREEN;
    CRS_POS_SOF;
    perror(s);
    exit(1);
}
