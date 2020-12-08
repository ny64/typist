#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "data.h"
#include "exit.h"
#include "input.h"
#include "output.h"
#include "terminal.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    tt.time = 60;
    tt.avg_word_length = 4.79;

    int no_filename = 1;
    for (int i = 1; i < argc; i++) {
        no_filename = parse_argument(argv[i]);
    }

    if (no_filename) {
        die("You have to provide a text file.", 1);
        exit(1);
    }

    init_terminal();
    parse_text();
    print_text();

    while (1) {
        process_keypress();
        if (check_timer() || tt.pos == tt.length) {
            print_score();
        }
    }

    return 0;
}
