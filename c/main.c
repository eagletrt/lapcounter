#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "lapcounter.h"
#include "geometry.h"

void main() {
    int x, y;
    Point point;
    FILE *file = fopen("track-8laps.txt", "r");
    while (!feof(file)) {

        fscanf(file, "%d %d\n", &x, &y);
        printf("x:%d\ty:%d\n", x, y);
        point = (Point){x, y};
        eval_point(point);
        getchar();

    }
    fclose(file);
    printf("Lapcounter C\n"); 
}