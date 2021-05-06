#include "lapcounter.h"
#include "geometry.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*clock_t start;
clock_t stop;
double interval;

void sw_start() {
    clock_t start = clock();
}

void sw_stop() {
    //printf("clocks per sec %d\n", CLOCKS_PER_SEC);
    clock_t stop = clock();
    interval = (double)(stop-start);
    printf("elapsed: %lu us\n", interval);
}*/

int check_proximity(LapCounter * lp) {
    double d1 = get_distance(&lp->last_point, &lp->start_points[0]);
    double d2 = get_distance(&lp->current_point, &lp->start_points[0]);
    return d1 < lp->proximity_thr && d2 < lp->proximity_thr;
}

int check_inclination(LapCounter * lp) {
    double current_vector_angle = get_vector_angle(&lp->current_vector);
    double angle_difference = (double)((int)(current_vector_angle - lp->start_vector_angle + 180) % 360 - 180);
    return fabs(angle_difference) <= lp->inclination_thr;
}

int check_overlap(LapCounter * lp) {
    Vector v1, v2;
    int overlap_p1cond, overlap_p2cond;
    set_vector(&v1, &lp->start_line.p1, &lp->current_vector.p1);
    set_vector(&v2, &lp->start_line.p1, &lp->current_vector.p2);
    overlap_p1cond = determinant(&lp->start_line, &v1) > 0;
    overlap_p2cond = determinant(&lp->start_line, &v2) > 0;
    return overlap_p1cond != overlap_p2cond;
}

int eval_point(LapCounter * lp, Point p, unsigned long timestamp) {

    double avg = 0;
    Vector temp;
    double proximity_temp;
    //while (p.x < 1000000.) { p.x *= 10.; }
    //while (p.y < 1000000.) { p.y *= 10.; }

    // sets current vector (p1: last point, p2: new point)
    lp->last_point = lp->current_point;
    lp->current_point = p;
    set_vector(&lp->current_vector, &lp->last_point, &lp->current_point);

    if (lp->start_index < START_COUNT) { // HANDLE FIRST POINTS FOR THE STARTLINE
        lp->start_points[lp->start_index] = p;
        lp->start_index++;

    } else if (lp->start_index == START_COUNT) { // CALCULATE START LINE AND START VECTOR
        for (int i = 0; i < START_COUNT-1; i++) {
            set_vector(&temp, &lp->start_points[i], &lp->start_points[i+1]);
            avg += get_vector_angle(&temp);
        }
        avg /= START_COUNT - 1;
        set_vector_from_angle(&lp->start_vector, &lp->start_points[0], avg);
        lp->start_vector_angle = avg;
        lp->start_index++;
        get_perpendicular_vector(&lp->start_vector, &lp->start_line);

    } else { // CALCULATE THE 3 RULES OF newlappity

        proximity_temp = get_vector_length(&lp->current_vector) *  lp->proximity_incr;
        if (proximity_temp > lp->proximity_thr) {
            lp->proximity_thr = proximity_temp;
        }

        if (check_proximity(lp) && check_inclination(lp) && check_overlap(lp)) {
            lp->laps[lp->lap_count] = timestamp;
            lp->lap_count++;
            return 1;
        }

    }

    return 0;
}

LapCounter * create_lapcounter(double proximity_incr, double inclination_thr) {
    LapCounter * lp = malloc(sizeof(LapCounter));
    lp->start_index = 0;
    lp->lap_count = 0;
    lp->proximity_thr = 0;
    lp->proximity_incr = proximity_incr;
    lp->inclination_thr = inclination_thr;
    return lp;
}

void reset_lapcounter(LapCounter * lp) {
    lp->start_index = 0;
    lp->lap_count = 0;
    lp->proximity_thr = 0;
}

void destroy_lapcounter(LapCounter * lp) {
    free(lp);
}