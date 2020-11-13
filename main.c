#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define CLR_SCREEN "\x1b[2J"
#define PC_SOF "\x1b[H"

/** Data **/

struct termios orig_termios;

void die(const char *s) {
    write(STDOUT_FILENO, CLR_SCREEN, 4); // position cursor at the top
    write(STDOUT_FILENO, PC_SOF, 3); // position cursor at the top
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
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
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

void printText(char *buffer) {
    printf("%s", buffer);
}

void refreshScreen() {
    write(STDOUT_FILENO, CLR_SCREEN, 4); // position cursor at the top
    write(STDOUT_FILENO, PC_SOF, 3); // position cursor at the top

    // further drawing
    //write(STDOUT_FILENO, PC_SOF, 3); // position cursor at the top
}

/** Input **/

void processKeypress() {
    char c = readKey();

    switch (c) {
        case CTRL_KEY('q'): 
        case CTRL_KEY('c'):
            write(STDOUT_FILENO, CLR_SCREEN, 4); // position cursor at the top
            write(STDOUT_FILENO, PC_SOF, 3); // position cursor at the top                 
            exit(0);
            break;
        case CTRL_KEY('r'):
            // TODO: restart test
            printf("refresh\n");
            break;
        default:
            printf("%c", c);
            // TODO: check if keypress checks with character under cursor
            // TODO: colors?
    }
}

/** Init **/

int main() {
    // TODO: get filename by cli flag
    char *filename = "test.txt";
    char *buffer = 0;
    long length;
    FILE * f = fopen(filename, "rb");

    if (f == NULL) {
        die(filename);
    } else {
        fseek(f, 0, SEEK_END); 
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer) fread(buffer, 1, length, f);
        fclose(f); 
    } // TODO: Add maximum file size 

    enableRawMode();

    while (1) {
        refreshScreen();
        printText(buffer);
        processKeypress();
    }
    
    return 0;
}
