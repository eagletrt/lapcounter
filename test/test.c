#include <stdio.h>
#include <assert.h>

#include "../src/lapcounter.h"
#include "worldmap/worldmap.h"

void main(int argc, char **argv) {

    double time_spent = 0.0;
    int bytes_read;     // to remove warning on fscanf
    double x, y;        // to read from file
    char mode;          // p: pixel, r: real

    lc_point_t point;   // to pass to evaluation
    lc_counter_t * lp = lc_init(NULL);  // initialization with default settings

    assert(argc >= 3);  // assert if command has filename and mode
    FILE *file = fopen(argv[1], "r");
    mode = argv[2][0];  // modes should be r/p

    clock_t begin = clock();

    if (file != NULL) {
            while (!feof(file)) {
            bytes_read = fscanf(file, "%lf %lf\n", &x, &y);
            if (mode == 'p') {
                point.x = x / 1000000 + 46.4331; // transforms pixel...
                point.y = y / 1000000 + 11.3129; // ...to fake coordinates
            } else if (mode == 'r') {
                point.x = x;
                point.y = y;
            }
            if (lc_eval_point(lp, &point)) // return 1 if new lap
                printf("BIP! ");
            if (argc == 4)      // if there is a third argument...
                worldmap(lp, argv[3]); // ...use it as API
            
        }
        fclose(file);
        printf("\nLAP COUNT: %d\n", lp->laps_count);
    } else {
        perror("Error");
    }

    lc_reset(lp); // reset object (removes everything but thresholds)
    lc_destroy(lp);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time is %f seconds\n", time_spent);
}