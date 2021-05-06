#ifndef LAPCOUNTER_H_INCLUDED
#define LAPCOUNTER_H_INCLUDED

#include "geometry.h"

#define START_COUNT 5
#define LAPS_COUNT 500

typedef struct {

    Point start_points[START_COUNT];
    int start_index;
    Vector start_line;
    Vector start_vector;
    double start_vector_angle;

    Point last_point;
    Point current_point;
    Vector current_vector;
    
    double proximity_thr;
    double proximity_incr;
    double inclination_thr;
    int lap_count;
    unsigned long laps[LAPS_COUNT];

} LapCounter;

LapCounter * create_lapcounter(double proximity_incr, double inclination_thr);

int eval_point(LapCounter * lp, Point p, unsigned long timestamp);

void reset_lapcounter(LapCounter * lp);

void destroy_lapcounter(LapCounter * lp);

#endif