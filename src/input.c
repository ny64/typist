#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "error.h"
#include "helper.h"
#include "input.h"
#include "terminal.h"

/** Input **/

void parseText() {
    tt.filename = "test.txt";
    tt.buffer = 0;
    FILE * f = fopen(tt.filename, "rb");

    unsigned int i, j;

    if (f == NULL) {
        die(tt.filename);
    } else {
        fseek(f, 0, SEEK_END);
        tt.length = ftell(f);
        fseek(f, 0, SEEK_SET);
        tt.buffer = malloc(tt.length);
        if (tt.buffer) fread(tt.buffer, 1, tt.length, f);
        fclose(f);
    }

    for (i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == ' ' && tt.buffer[i+1] == ' ') {
            for (j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j + 1];
            }
            tt.length--;
            i--;
        } else if (tt.buffer[i + 1] == '\n' && tt.buffer[i] == ' ') {
            for (j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j + 1];
            }
            tt.length--;
            i--;
        }
    }
}

void processKeypress() {
    char c = readKey();

    FONT_CLR_DEF;

    switch (c) {
        case CTRL_KEY('q'):
        case CTRL_KEY('c'):
            CLR_SCREEN;
            CRS_POS_SOF;
            exit(0);
        case CTRL_KEY('r'):
            // TODO: restart test
            die("TODO: Restart Test");
            break;
        case 127:
            if (tt.pos == 0) {
                break;
            } else if (tt.buffer[tt.pos - 1] == '\n') {
                int i = 2;
                CRS_POS_UP;
                while (tt.buffer[tt.pos - i] != '\n' && tt.pos - i != 0) {
                    CRS_POS_F;
                    i++;
                }
                if (tt.pos - i == 0) {
                    CRS_POS_F;
                }
                tt.pos--;
                PRINT_TO_SCREEN("\u23CE");
                CRS_POS_B;
                break;
            } else {
                CRS_POS_B;
                tt.pos--;
                PRINT_FROM_BUFFER;
                CRS_POS_B;
                break;
            }
        case '\r':
            if (tt.buffer[tt.pos] == '\n') {
                // print green return arrow
                FONT_CLR_GRN;
                PRINT_TO_SCREEN("\u23CE\n");
                tt.pos++;
            }
            break;
        default:
            if (iscntrl(c) && c != '\r') break;
            if (c == tt.buffer[tt.pos]) {
                FONT_CLR_GRN;
                PRINT_FROM_BUFFER;
            } else {
                FONT_CLR_RED;
                if (tt.buffer[tt.pos] == '\n') {
                    PRINT_TO_SCREEN("\u23CE\n");
                } else {
                    PRINT_FROM_BUFFER;
                }
            }
            tt.pos++;

            FONT_CLR_DEF;

            if (tt.pos == tt.length) {
                // TODO: calculate score
                die("TODO: Calc score");
            }
    }
}