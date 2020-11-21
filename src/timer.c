#include <time.h>
#include <stdio.h>

#include "data.h"

time_t start, stop;

void start_timer() {
    start = time(0);
}

int check_timer() {
    tt.elapsed_time = time(0) - start;
    if (tt.elapsed_time > tt.time) {
        return 1;
    }

    return 0;
}

void stop_timer() {
    stop = time(0);
}

