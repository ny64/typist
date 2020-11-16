#include "input.h"
#include "output.h"
#include "terminal.h"

/** Init **/

int main() {
    enableRawMode();
    refreshScreen();
    parseText("test.txt");
    printText();

    while (1) {
        processKeypress();
    }

    return 0;
}

