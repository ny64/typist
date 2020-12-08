#ifndef DATA_H
#define DATA_H

struct typingtest {
    char *filename;
    char *buffer;
    char *buffer_score;
    unsigned int time;
    unsigned long elapsed_time;
    unsigned int length;
    unsigned int pos;
};

extern struct typingtest tt;
extern struct termios orig_termios;

#endif //DATA_H
