#ifndef OUTPUT_H
#define OUTPUT_H

extern void print_help();
extern void refresh_screen();
extern void print_text();
extern void print_from_buffer();
extern void jump_to_next_word();
extern void jump_to_next_line();
extern void del();
extern void del_word();
extern void del_to_prev_line();

#endif //OUTPUT_H
