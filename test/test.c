#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "../src/lapcounter.h"

void main() {
    unsigned long time = 0; // debug purposes 
    int bytes_read; // to remove warning on fscanf
    double x, y; // to read from file
    lc_point_t point; // to pass to evaluation

    // these already are optimal values
    lc_counter_t * lp = lc_init(NULL); // initialization with 105% proximity increment, +-45° inclination thr.

    FILE *file = fopen("example_tracks/track-8lap-tangled.txt", "r");
    while (!feof(file)) {
        bytes_read = fscanf(file, "%lf %lf\n", &x, &y);
        point.x = x / 1000000 + 46.4331; // transforms pixel...
        point.y = y / 1000000 + 11.3129; // ...to fake coordinates
        if (lc_eval_point(lp, &point)) // return True if new lap
            printf("BIP!\n");
        time++;
    }
    fclose(file);

    printf("LAP COUNT: %d\n", lp->laps_count);

    lc_reset(lp); // reset object (removes everything but thresholds)
    lc_destroy(lp);
}