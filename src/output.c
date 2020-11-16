#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "helper.h"

void refreshScreen() {
    CLR_SCREEN;
    CRS_POS_SOF;
}

void printText() {
    unsigned int i;
    for (i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == '\n') {
            printf("\u23CE\n");
        } else {
            printf("%c", tt.buffer[i]);
        }
    }
    CRS_POS_SOF;
}
