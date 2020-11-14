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
#define CRS_POS_D write(STDOUT_FILENO, "\x1b[B", 3); \
                  write(STDOUT_FILENO, "\x1b[1000D", 4) // position cursor down 

/** Data **/

struct termios orig_termios;

struct typingtest {
    char *filename;
    char *buffer;
    int pos;
    int mistakes;
};

struct typingtest tt;

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
//    raw.c_cc[VMIN] = 0;
//    raw.c_cc[VTIME] = 1;
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
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read"); 
    }
    return c;
}

/** Output **/

void printText() {
    printf("%s", tt.buffer);
    CRS_POS_SOF;
}

void refreshScreen() {
    CLR_SCREEN;
    CRS_POS_SOF;

    // further drawing
    //CRS_POS_SOF;
}

/** Input **/

void processKeypress() {
    char c = readKey();
    //char current_char = 'x';

    switch (c) {
        case CTRL_KEY('q'): 
        case CTRL_KEY('c'):
            CLR_SCREEN;
            CRS_POS_SOF;
            exit(0);
        case CTRL_KEY('r'):
            // TODO: restart test
            printf("refresh\n");
            break;
        case 32: // space
            CRS_POS_F;
            break;
        case 10: // nextline
            CRS_POS_D; // TODO: not working
            break;
        case 127:  // backspace
            CRS_POS_B;
            break;
        default:
            if (iscntrl(c)) {
                break;
            }
            write(STDOUT_FILENO, &c, 1);
            // TODO: check if keypress checks with character under cursor
            // TODO: colors
    }
}

/** Init **/

int main() {
    // TODO: get filename by cli flag

    tt.filename = "test.txt";
    tt.buffer = 0;
    long length;
    FILE * f = fopen(tt.filename, "rb");

    if (f == NULL) {
        die(tt.filename);
    } else {
        fseek(f, 0, SEEK_END); 
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        tt.buffer = malloc(length);
        if (tt.buffer) fread(tt.buffer, 1, length, f);
        fclose(f); 
    } // TODO: Add maximum file size 

    enableRawMode();
    refreshScreen();
    printText();

    while (1) {
        processKeypress();
    }
    
    return 0;
}
