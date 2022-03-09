#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file = fopen("coordsProva1Lap.txt", "r");
    while (!feof(file)) {
        double lat, lon;
        fscanf(file, "%f %f\n", &lat, &lon);
        printf("%f %f\n", lat, lon);
    }
    return 0;
}
