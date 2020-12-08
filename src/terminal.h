#ifndef TERMINAL_H
#define TERMINAL_H

void disable_raw_mode();
void enable_raw_mode();
void init_terminal();
char read_key();

#endif //TERMINAL_H
