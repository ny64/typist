#ifndef HELPER_H
#define HELPER_H

#define CTRL_KEY(k) ((k) & 0x1f)

// print to screen
#define PRINT_TO_SCREEN(s) write(STDOUT_FILENO, s, strlen(s))

// clear screen
#define CLR_SCREEN PRINT_TO_SCREEN("\x1b[2J")

// move cursor
#define CRS_POS_TOP PRINT_TO_SCREEN("\x1b[H")
#define CRS_POS_F PRINT_TO_SCREEN("\x1b[C")
#define CRS_POS_B PRINT_TO_SCREEN("\x1b[D")
#define CRS_POS_UP PRINT_TO_SCREEN("\x1b[A")
#define CRS_POS_DOWN PRINT_TO_SCREEN("\x1b[B")

// font colors
#define FONT_CLR_DEF PRINT_TO_SCREEN("\033[0m")
#define FONT_CLR_GRN PRINT_TO_SCREEN("\033[0;32m")
#define FONT_CLR_RED PRINT_TO_SCREEN("\033[0;31m")
#define FONT_CLR_YLW PRINT_TO_SCREEN("\033[0;33m")

#endif //HELPER_H
