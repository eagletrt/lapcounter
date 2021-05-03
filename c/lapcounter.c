#include "lapcounter.h"
#include "geometry.h"
#include <stdio.h>

#define START_COUNT 5

Point start_points[START_COUNT];
int start_index = 0;
Point last_point;
Point current_point;

Vector start_line;
Vector start_vector;
double start_vector_angle;
Vector current_vector;
double current_vector_angle;
double angle_difference;

double proximity_thr = 40;
double inclination_thr = 45;


int eval_point(Point p) {

    double avg = 0;
    Vector temp;

    if (start_index < START_COUNT) { // HANDLE FIRST POINTS FOR THE STARTLINE
        printf("adding to start_points\n");
        start_points[start_index] = p;
        start_index++;

    } else if (start_index == START_COUNT) { // CALCULATE START LINE AND START VECTOR
        printf("calculatin start vector\n");
        for (int i = 0; i < START_COUNT-1; i++) {
            set_vector(&temp, &start_points[i], &start_points[i+1]);
            printf("%f\n", get_vector_angle(&temp));
            avg += get_vector_angle(&temp);
        }
        avg /= START_COUNT - 1;
        printf("avg: %f\n", avg);
        print_point(&start_points[0]);
        set_vector_from_angle(&start_vector, &start_points[0], avg);
        start_vector_angle = avg;
        print_vector(&start_vector);
        start_index++;

    } else { // CALCULATE THE 3 RULES OF newlappity

        last_point = current_point;
        current_point = p;
        set_vector(&current_vector, &last_point, &current_point);

        // debug
        printf("angle: %f\n", get_vector_angle(&current_vector));

        // 1 - proximity
        if (get_distance(&last_point, &start_points[0]) < proximity_thr &&
            get_distance(&current_point, &start_points[0]) < proximity_thr) {
                printf("proximity passed\n");
        } else {
            printf("proximity rejected\n");
            return 0;
        }
        
        // 2 - inclination
        current_vector_angle = get_vector_angle(&current_vector);
        angle_difference = (double)(((int)current_vector_angle - (int)start_vector_angle + 180) % 360 - 180);
        printf("%f %f %f\n", current_vector_angle, start_vector_angle, angle_difference);

        // 3 - overlap




    }

    return 0;

}
