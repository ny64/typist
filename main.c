#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

// print to screen
#define PTS(s) write(STDOUT_FILENO, s, strlen(s))
#define PRINT_FROM_BUFFER write(STDOUT_FILENO, &tt.buffer[tt.pos], 1) 

// clear screen
#define CLR_SCREEN PTS("\x1b[2J") 

// move cursor
#define CRS_POS_SOF PTS("\x1b[H") 
#define CRS_POS_F PTS("\x1b[C") 
#define CRS_POS_B PTS("\x1b[D") 
#define CRS_POS_UP PTS("\x1b[A") 

// font colors 
#define FONT_CLR_DEF PTS("\033[0m")
#define FONT_CLR_GRN PTS("\033[0;32m")
#define FONT_CLR_RED PTS("\033[0;31m")

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
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
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
                PTS("\u23CE");
                CRS_POS_B;
                break;
            } else {
                CRS_POS_B;
                tt.pos--;
                PRINT_FROM_BUFFER;
                CRS_POS_B;
                break;
            }
        case '\r':
            if (tt.buffer[tt.pos] == '\n') {
                // print green return arrow
                FONT_CLR_GRN;
                PTS("\u23CE\n");
                tt.pos++;
            }
            break;
        default:
            if (iscntrl(c) && c != '\r') break;
            if (c == tt.buffer[tt.pos]) {
                FONT_CLR_GRN;
                PRINT_FROM_BUFFER;
            } else {
                FONT_CLR_RED;
                if (tt.buffer[tt.pos] == '\n') {
                    PTS("\u23CE\n");
                } else {
                    PRINT_FROM_BUFFER;
                }
            }
            tt.pos++;

        FONT_CLR_DEF;

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

