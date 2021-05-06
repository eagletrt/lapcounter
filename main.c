#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "lapcounter.h"
#include "geometry.h"

// giro
// tempo_iniziale, tempo_fine, sessione, n_giro

// documents
// + n_giro

void main() {

    unsigned long time = 0; // debug purposes 
    int bytes_read; // to remove warning on fscanf
    double x, y; // to read from file
    Point point; // to pass to evaluation

    // these already are optimal values
    LapCounter * lp = create_lapcounter(1.05, 45); // initialization with 105% proximity increment, +-45° inclination thr.

    FILE *file = fopen("example_tracks/track-8lap-tangled.txt", "r");
    while (!feof(file)) {
        bytes_read = fscanf(file, "%lf %lf\n", &x, &y);
        point.x = x / 1000000 + 46.4331; // transforms pixel...
        point.y = y / 1000000 + 11.3129; // ...to fake coordinates
        if (eval_point(lp, point, time)) // return True if new lap
            printf("BIP!\n");
        time++;
    }
    fclose(file);

    printf("LAP COUNT: %d\n", lp->lap_count);
    for (int i = 0; i < lp->lap_count; i++) {
        printf("lap %d completed, timestamp:%lu\n", i, lp->laps[i]);
    }

    reset_lapcounter(lp); // reset object (removes everything but thresholds)
}