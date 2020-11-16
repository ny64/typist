#ifndef DATA_H
#define DATA_H

struct typingtest {
    char *filename;
    char *buffer;
    unsigned int length;
    unsigned int pos;
    unsigned int mistakes;
};
extern struct typingtest tt;
extern struct termios orig_termios;

#endif //DATA_H
