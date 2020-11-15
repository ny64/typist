#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

#define CLR_SCREEN write(STDOUT_FILENO, "\x1b[2J", 4) // clear screen
#define CRS_POS_SOF write(STDOUT_FILENO, "\x1b[H", 3) // position cursor at top
#define CRS_POS_F write(STDOUT_FILENO, "\x1b[C", 3) // position cursor forward
#define CRS_POS_B write(STDOUT_FILENO, "\x1b[D", 3) // position cursor backward
#define CRS_POS_UP write(STDOUT_FILENO, "\x1b[A", 3) // position cursor up 

/** Data **/

struct typingtest {
    char *filename;
    char *buffer;
    unsigned long length;
    unsigned int pos;
    unsigned int mistakes;
};

struct typingtest tt;

struct termios orig_termios;

/** Exiting **/

void die(const char *s) {
    CLR_SCREEN;
    CRS_POS_SOF;
    perror(s);
    exit(1);
}

/** Terminal **/

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    // store current terminal attributes
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    // alter terminal attributes
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    /** Reference:
     *  c_iflag
     *    IXON: disables Ctrl-s and Ctrl-q behaviour
     *    ICRNL: disables Ctrl-M
     *  c_oflag
     *    OPOST: disables moving cursor to the start on a new line
     *  c_lflag
     *    ECHO: stop input being printed to terminal
     *    ICANON: turn off canonical mode 
     *    ISIG: disables Ctrl-c and Ctrl-z behaviour
     *    IEXTEN: disables Ctrl-V behaviour
     *  c_cc (arr)
     *    VMIN: minimum number of bytes before read() returns
     *    VTIME: maximum time to wait before read() returns (1 = 1/10s)
     **/
    // set terminal attributes
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char readKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) 
        if (nread == -1 && errno != EAGAIN) die("read"); 
    return c;
}

/** Output **/

void refreshScreen() {
    CLR_SCREEN;
    CRS_POS_SOF;
}

/** Input **/

void processKeypress() {
    char c = readKey();

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
            // TODO: Jump to line above
            if (tt.pos == 0) {
                break;
            } else if (tt.buffer[tt.pos - 1] == '\n') {
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
                write(STDOUT_FILENO, "\u23CE", 3);
                CRS_POS_B;
                break;
            } else {
                CRS_POS_B;
                tt.pos--;
                write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
                CRS_POS_B;
                break;
            }
        case '\r':
            if (tt.buffer[tt.pos] == '\n') {
                // print green return arrow
                write(STDOUT_FILENO, "\033[0;32m\u23CE\n\033[0m", 15);
                tt.pos++;
            }
            break;
        default:
            if (iscntrl(c) && c != '\r') break;
            if (c == tt.buffer[tt.pos]) {
                write(STDOUT_FILENO, "\033[0;32m", 7);
                write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
                write(STDOUT_FILENO, "\033[0m", 4);
            } else {
                write(STDOUT_FILENO, "\033[0;31m", 7);
                if (tt.buffer[tt.pos] == '\n') {
                    write(STDOUT_FILENO, "\u23CE", 3);
                    write(STDOUT_FILENO, "\n", 1);
                } else {
                    write(STDOUT_FILENO, &tt.buffer[tt.pos], 1);
                }
                write(STDOUT_FILENO, "\033[0m", 4);
            }
            tt.pos++;

        if (tt.pos == tt.length) {
            // TODO: calculate score
            die("TODO: Calc score");
        }
    }
}

/** Init **/

void parseText() {
    tt.filename = "test.txt";
    tt.buffer = 0;
    FILE * f = fopen(tt.filename, "rb");

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
}

void printText() {
    for (int i = 0; i < tt.length; i++) {
        if (tt.buffer[i] == ' ' && tt.buffer[i+1] == ' ') {
        // drop double spaces
            for (int j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j + 1];
            }
            tt.length--;
            i--;
        } else if (tt.buffer[i + 1] == '\n' && tt.buffer[i] == ' ') { 
        // drop spaces before line breaks
            for (int j = i; j < tt.length - 1; j++) {
                tt.buffer[j] = tt.buffer[j + 1];
            }
            tt.length--;
            i--;
        } else {
            if (tt.buffer[i] == '\n') {
                printf("\u23CE\n");
            } else {
                printf("%c", tt.buffer[i]);
            }
        }
    }
    CRS_POS_SOF;
}

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

