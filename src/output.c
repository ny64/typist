#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "helper.h"
#include "output.h"

void del() {
    CRS_POS_B;
    tt.pos--;
    print_from_buffer();
    CRS_POS_B;
}

void del_to_prev_line() {
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
}

void jump_to_next_line() {
    write(STDOUT_FILENO, "\u23CE\n", strlen("\u23CE\n"));
}

void jump_to_next_word() {
    FONT_CLR_RED;
    while (tt.buffer[tt.pos] != ' ' && tt.buffer[tt.pos] != '\n') {
        print_from_buffer();
        tt.buffer_score[tt.pos] = -1;
        tt.pos++;
    }
    if (tt.buffer[tt.pos] == ' ') {
        FONT_CLR_GRN;
        print_from_buffer();
        tt.buffer_score[tt.pos] = 1;
        tt.pos++;
    }
}

void print_from_buffer() {
    write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
}

void print_help() {
    printf("Usage: typist [OPTION]... [FILE]\n");
    printf("Start a typing speed test with the specified .txt FILE.\n\n");
    printf("  -h, --help\t\t\t\tprint this message\n\n");
    printf("  -t [TIME], --time [TIME]\t\tset the duration of the test;\n"\
            "\t\t\t\t\tTIME sets the time in seconds;\n"\
            "\t\t\t\t\te.g. -t 600 for a 10 minute typing test;\n"\
            "\t\t\t\t\tDefault is set to 60 seconds\n\n");
    printf("  --average-word-length [LENGTH]\tset average word length "\
            "for calculating WPM;\n"\
            "\t\t\t\t\tLENGTH specifies the average length of a word;\n"\
            "\t\t\t\t\te.g. --average-word-length 6.0 for average word length "\
            "in the german language;\n"\
            "\t\t\t\t\tDefault is set to 4.79 (default word length in the "\
            "english language)\n");
}

void print_score() {
    float wpm, kpm;
    int keystrokes = 0;
    int correct = 0;
    int wrong = 0;

    for (int i = 0; tt.buffer_score[i] != 0; i++) {
        keystrokes++;
        if (tt.buffer_score[i] == 1) correct++;
        else wrong++;
    }

    kpm = 60 / (float)tt.elapsed_time * (float)correct;
    wpm = kpm / tt.avg_word_length;

    CLR_SCREEN;
    CRS_POS_TOP;
    FONT_CLR_DEF;
    printf("   __            _     __\n"); 
    printf("  / /___ _____  (_)__ / /_\n");
    printf(" / __/ // / _ \\/ (_-</ __/\n");
    printf(" \\__/\\_, / .__/_/___/\\__/\n");
    printf("    /___/_/\n\n");
    printf(" The terminal \033[1mtypi\033[0mng \033[1ms\033[0mpeed "\
            "\033[1mt\033[0mest\n\n");
    printf(" Keystrokes:\t(");
    printf("\x1b[32m%d", correct);
    printf("\x1b[0m|");
    printf("\x1b[31m%d", wrong);
    printf("\x1b[0m) %d\n", keystrokes);
    printf(" KPM:\t\t%.2f\n", kpm);
    printf(" WPM:\t\t%.2f\n\n", wpm);
    exit(0);
}

void print_text() {
    CLR_SCREEN;
    CRS_POS_TOP;
    FONT_CLR_DEF;    
    int text_rows = 1;
    int text_cols = 1;
    int last_line_pos = 0;
    for (; tt.print_index < tt.length; tt.print_index++) {
        if (tt.buffer[tt.print_index] == '\n') {
            write(STDOUT_FILENO, "\u23CE\n", strlen("\u23CE\n"));
        } else {
            write(STDOUT_FILENO, &tt.buffer[tt.print_index], 1);
        }
        if (tt.buffer[tt.print_index] == '\n' || text_cols == tt.term_cols) {
            text_rows++;
            if (text_rows == tt.term_rows) {
                tt.print_index++;
                CRS_POS_TOP;
                tt.print_index = last_line_pos;
                return;
            } else {
                last_line_pos = tt.print_index + 1;
            }
            text_cols = 0;
        }
        text_cols++;
    }
    CRS_POS_TOP;
}

void refresh_screen() {
    CLR_SCREEN;
    CRS_POS_TOP;
}

