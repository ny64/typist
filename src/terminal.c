#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "data.h"
#include "exit.h"
#include "output.h"
#include "terminal.h"
#include "timer.h"

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr", 0);
}

void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr", 0);
    atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr", 0);
}

int get_window_size(int *rows, int *cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void init_terminal() {
    enable_raw_mode();
    if (get_window_size(&tt.term_rows, &tt.term_cols) == -1)
        die("get_window_size", 0);
    refresh_screen();
}

int first_key_read = 0;

char read_key() {
    int nread;
    char c;

    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        printf("READ KEY\n");
        if (nread == -1 && errno != EAGAIN) die("read", 0);
    }

    if (!first_key_read) {
        first_key_read = 1;
        start_timer();
    }

    return c;
}
