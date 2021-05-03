#include "lapcounter.h"
#include "geometry.h"

#define START_COUNT 5
#define CURRENT_COUNT 3

Point start_points[START_COUNT];
int start_index = 0;
Point current_point[CURRENT_COUNT];
int current_index = 0;

Vector start_line;
Vector start_vector;
Vector current_vector;

int eval_point(Point p) {

    if (start_index < START_COUNT) { // HANDLE FIRST POINTS FOR THE STARTLINE
        start_points[start_index] = p;
        start_index++;

    } else if (start_index == START_COUNT) { // CALCULATE START LINE AND START VECTOR
        
        

    } else { // CALCULATE THE 3 RULES OF newlappity

        // 1 - adjacency

        // 2 - inclination

        // 3 - overlap
    }
}
