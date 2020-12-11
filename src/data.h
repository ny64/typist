#ifndef DATA_H
#define DATA_H

struct typingtest {
    char *filename;
    float avg_word_length;
    unsigned int time;

    int term_rows;
    int term_cols;

    char *buffer;
    char *buffer_score;
    char *is_new_line;
    char *is_new_page;
    unsigned int length;
    unsigned int pos;
    unsigned int print_index;
    
    unsigned long elapsed_time;
};

extern struct typingtest tt;
extern struct termios orig_termios;

#endif //DATA_H
