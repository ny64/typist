#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "data.h"
#include "exit.h"
#include "input.h"
#include "output.h"
#include "terminal.h"

int main(int argc, char *argv[]) {
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "f:t:")) != -1) {
        switch (c) {
            case 'f':
                tt.filename = optarg;
                break;
            case 't':
                tt.time = atoi(optarg);
                break;
            case '?':
                if (optopt == 'c') {
                    fprintf(stderr, 
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf(stderr, 
                            "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr, 
                            "Unknown option character `\\x%x'.\n", optopt);
                }
            return 1;
            default:
                abort();
        }
    }

    enable_raw_mode();
    refresh_screen();
    parse_text("test.txt");
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

