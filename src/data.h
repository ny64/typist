#ifndef DATA_H
#define DATA_H

struct typingtest {
    char *filename;
    char *buffer;
    unsigned int time;
    unsigned int length;
    unsigned int pos;
};
extern struct typingtest tt;
extern struct termios orig_termios;

#endif //DATA_H
