#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data.h"
#include "helper.h"

void print_help() {
    printf("Usage: typist [OPTION]... [FILE]\n");
    printf("Start a typing speed test with the specified .txt FILE.\n\n");
    printf("  -t [TIME], --time [TIME]\t\tset the duration of the test;\n"\
            "\t\t\t\t\tTIME sets the time in seconds\n");
    printf("  -r [AMOUNT], --random [AMOUNT]\trandomize words and cut "\
            "punctuation marks in text file;\n"\
            "\t\t\t\t\tAMOUNT specifies the amount of words\n");
}

void refresh_screen() {
    CLR_SCREEN;
    CRS_POS_TOP;
}

void print_text() {
    unsigned int i;
    for (i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == '\n') {
            write(STDOUT_FILENO, "\u23CE\n", strlen("\u23CE\n"));
        } else {
            write(STDOUT_FILENO, &tt.buffer[i], 1);
        }
    }
    CRS_POS_TOP;
}

void print_from_buffer() {
    write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
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

void jump_to_next_line() {
    write(STDOUT_FILENO, "\u23CE\n", strlen("\u23CE\n"));
}

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
    wpm = kpm / 4.79;  // average word length in the english language

    CLR_SCREEN;
    CRS_POS_TOP;
    FONT_CLR_DEF;
    printf("Keystrokes:\t(");
    printf("\x1b[32m%d", correct);
    printf("\x1b[0m|");
    printf("\x1b[31m%d", wrong);
    printf("\x1b[0m) %d\n", keystrokes);
    printf("KPM:\t\t%.2f\n", kpm);
    printf("WPM:\t\t%.2f\n", wpm);
    exit(0);
}

