#include <ctype.h>
#include <errno.h>
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
#include "timer.h"

void parse_text() {
    tt.buffer = 0;
    tt.buffer_score = 0;
    tt.is_new_line = 0;
    tt.is_new_page = 0;
    FILE * f = fopen(tt.filename, "rb");

    unsigned int i, j;

    if (f == NULL) {
        die(tt.filename, 0);
    }

    fseek(f, 0, SEEK_END);
    tt.length = ftell(f);
    fseek(f, 0, SEEK_SET);
    tt.buffer = malloc(tt.length);
    tt.buffer_score = malloc(tt.length);
    tt.is_new_line = malloc(tt.length);
    tt.is_new_page = malloc(tt.length);
    if (tt.buffer) fread(tt.buffer, 1, tt.length, f);
    fclose(f);

    for (i = 0; i < tt.length; i++) {
        tt.buffer_score[i] = 0;
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

    int text_rows = 1;
    int text_cols = 1;
    for (i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == '\n' || text_cols == tt.term_cols) {
            text_rows++;
            text_cols = 0;
            tt.is_new_line[i+1] = 1;
        } else {
            tt.is_new_line[i+1] = 0;
        }
        if (text_rows == tt.term_rows - 1 && text_cols == 0) {
            tt.is_new_page[i+1] = 1;
        } else {
            tt.is_new_page[i+1] = 0;
        }
        text_cols++;
    }
}

void process_keypress() {
    char c = read_key();

    FONT_CLR_DEF;

    switch (c) {
        case CTRL_KEY('c'):
        case CTRL_KEY('q'):
            CLR_SCREEN;
            CRS_POS_TOP;
            exit(0);
        case CTRL_KEY('r'):
            refresh_screen();
            parse_text();
            print_text();
            tt.pos = 0;
            first_key_read = 0;
            break;
        case 127:
            if (tt.pos == 0 || tt.is_new_page[tt.pos]) {
                break;
            } else if (tt.buffer[tt.pos - 1] == '\n') {
                del_to_prev_line();
                tt.buffer_score[tt.pos] = 0;
                break;
            } else {
                del();
                break;
            }
        /* This block changes the behaviour of pressing space; skip over 
         * current word on pressing space
        case 32:
            if (tt.buffer[tt.pos] == '\n'){
                break;
            } else if (tt.buffer[tt.pos] != ' ') {
                jump_to_next_word();
            } else {
                print_from_buffer();
                tt.buffer_score[tt.pos] = 1;
                tt.pos++;
            }
            break;
        */
        case '\r':
            if (tt.buffer[tt.pos] != '\n') break;
            FONT_CLR_GRN;
            jump_to_next_line();
            tt.buffer_score[tt.pos] = 1;
            tt.pos++;
            break;
        default:
            if (iscntrl(c) && c != '\r') {
                break;
            } else if (c == tt.buffer[tt.pos]) {
                FONT_CLR_GRN;
                print_from_buffer();
                tt.buffer_score[tt.pos] = 1;
            } else {
                FONT_CLR_RED;
                if (tt.buffer[tt.pos] == '\n') {
                    jump_to_next_line();
                } else {
                    print_from_buffer();
                }
                tt.buffer_score[tt.pos] = -1;
            }
            tt.pos++;
    }

    if (tt.pos == tt.print_index) {
        print_text();
    }
}

