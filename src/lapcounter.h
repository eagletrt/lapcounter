#ifndef _LC_LAPCOUNTER_H
#define _LC_LAPCOUNTER_H

/** INCLUDES **/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "./utils/vector/vector.h"

/** STRUCTS **/

/**
 * @brief The struct for the options passed to the lc_counter
 */
typedef struct _lc_counter_config_t {
    /** If not NULL, the start_points_count that will be assigned to the lc_counter. **/ 
    size_t *start_points_count;
    /** If not NULL, the proximity_incr that will be assigned to the lc_counter. **/ 
    double *proximity_incr;
    /** If not NULL, the inclination_thr that will be assigned to the lc_counter. **/ 
    double *inclination_thr;
} lc_counter_config_t;

/**
 * @brief The struct containing the lap info
 */
typedef struct _lc_counter_t
{
    /** The first points, used to determine the start line and vector **/
    lc_point_t *start_points;
    /** The number of first points that will be used to calculate the first line **/ 
    size_t start_points_count;
    /** The index of the current point, used to understand if the start line is to be evaluated and incremented only in the beginning **/ 
    size_t start_point_index;
    /** The start vector, a vector that starts from the first point and has as direction the average angle of the first points' vectors **/
    lc_vector_t start_vector;
    /** The start line, vector perpendicular to the start vector **/
    lc_vector_t start_line;
    /** The start vector angle, which is the average angle of the first vectors **/
    double start_vector_angle;

    /** The last added point **/
    lc_point_t last_point;
    /** The current added point **/
    lc_point_t current_point;
    /** The current vector (between last and current point) **/
    lc_vector_t current_vector;
    /** Last result of proximity evaluation **/
    int last_proximity_result;
    /** Last result of inclination evaluation **/
    int last_inclination_result;
    /** Last result of overlap evaluation **/
    int last_overlap_result;

    double proximity_thr;
    double proximity_incr;
    double inclination_thr;

    /** The number of laps that have been detected **/
    int laps_count;
} lc_counter_t;

/** EXPORTED FUNCTIONS **/

/**
 * @brief Creates a leap counter
 * 
 * @param config The configurations for the leap counter.
 * @return The created leap counter
 */
lc_counter_t *lc_init(lc_counter_config_t* config);

/**
 * @brief Adds a point to the leap counter and returns if there is a new leap
 * 
 * @param lp The leap counter reference.
 * @param p The new point reference.
 * @return int 1 if there is a new leap
 */
int lc_eval_point(lc_counter_t *lp, lc_point_t *p, lc_counter_t *lp_inclination);

/**
 * @brief Resets the leap counter, as it were after the init function
 * 
 * @param lp The ref to the leap counter
 */
void lc_reset(lc_counter_t *lp);

/**
 * @brief Deallocates a leap counter
 * 
 * @param lp The leap counter to deallocate
 */
void lc_destroy(lc_counter_t *lp);

#endif
