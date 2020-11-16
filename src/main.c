#include "data.h"
#include "exit.h"
#include "input.h"
#include "output.h"
#include "terminal.h"

/** Init **/

int main() {
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

