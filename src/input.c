#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "exit.h"
#include "helper.h"
#include "input.h"
#include "output.h"
#include "terminal.h"

void parse_text() {
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
        if ((tt.buffer[i+1] == ' ' && tt.buffer[i] == ' ')
                || (tt.buffer[i+1] == '\n' && tt.buffer[i] == ' ')
                || (tt.buffer[i+1] == '\n' && tt.buffer[i] == '\n')) {
            for (j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j+1];
            }
            tt.length--;
            i--;
        } 
    }
}

void process_keypress() {
    char c = read_key();

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
                del_to_prev_line();            
            } else {
                del();
            }
            break;
        case 32:
            if (tt.buffer[tt.pos] == '\n'){
                break; // TODO: wrong keypress?
            } else if (tt.buffer[tt.pos] != ' ') {
                jump_to_next_word();
            } else {
                print_from_buffer();
                tt.pos++;
            }
            break;
        case '\r':
            if (tt.buffer[tt.pos] != '\n') break;
            FONT_CLR_GRN;
            jump_to_next_line();
            tt.pos++;
            break;
        default:
            if (iscntrl(c) && c != '\r') { 
                break;
            } else if (c == tt.buffer[tt.pos]) {
                FONT_CLR_GRN;
                print_from_buffer();
            } else {
                FONT_CLR_RED;
                if (tt.buffer[tt.pos] == '\n') {
                    jump_to_next_line();
                } else {
                    print_from_buffer();
                }
            }
            tt.pos++;
    }
}
