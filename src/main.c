#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "data.h"
#include "exit.h"
#include "input.h"
#include "output.h"
#include "terminal.h"

int main(int argc, char *argv[]) {

    int no_filename = 1;
    for (int i = 1; i < argc; i++) {
        no_filename = parse_argument(argv[i]);
    }
    if (no_filename) {
        printf("You have to provide a textfile for the typingtest.\n\n");
        print_help();
        exit(1);
    }

    enable_raw_mode();
    refresh_screen();
    parse_text(tt.filename);
    print_text();

    while (1) {
        process_keypress();
        if (tt.pos == tt.length) {
            // TODO: calculate score
            die("TODO: Calc score");
        }
    }

    return 0;
}

