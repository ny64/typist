#ifndef TERMINAL_H
#define TERMINAL_H

extern void disable_raw_mode();
extern void enable_raw_mode();
extern void init_terminal();
extern char read_key();
extern int first_key_read;

#endif //TERMINAL_H
