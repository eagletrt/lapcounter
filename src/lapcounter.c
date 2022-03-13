#include "lapcounter.h"

/** HELPER VARS **/

static const double _LC_DEFAULT_PROXIMITY_INCR = 1.85;
static const double _LC_DEFAULT_INCLINATION_THR = 45;
static const size_t _LC_DEFAULT_START_POINTS_COUNT = 50;
#define _LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH 20

/** HELPER FUNCTION SIGNATURES **/

// Updates the current vector by adding a new point
static void _update_current_vector(lc_counter_t *lp, lc_point_t *p);
// Updates the startline points by adding a new point
static void _update_startline_points(lc_counter_t *lp, lc_point_t *p);
// Calculates the start line and the start vector, used when the start points are all gathered
static void _calc_startline(lc_counter_t *lp, lc_point_t *p);
// Updates the proximity thr after that the current vector has been updated
static void _update_proximity_thr(lc_counter_t *lp);
// Resets to False (0) the results from the last evaluation
static void _reset_last_results(lc_counter_t *lp);

// Checks if the last point is very near to the first point
static int _check_proximity(lc_counter_t *lp);
// Checks if the inclination of the last vector is ok with the iniitial vector
static int _check_inclination(lc_counter_t *lp);
// Checks if the start line has been passed
static int _check_overlap(lc_counter_t *lp);

/** EXPORTED FUNCTIONS **/

lc_counter_t *lc_init(lc_counter_config_t *config)
{
    lc_counter_t *lp = malloc(sizeof(lc_counter_t));

    lp->start_point_index = 0;
    lp->laps_count = 0;
    lp->proximity_thr = 0;
    lp->proximity_incr = config == NULL || config->proximity_incr == NULL ? _LC_DEFAULT_PROXIMITY_INCR : *config->proximity_incr;
    lp->inclination_thr = config == NULL || config->inclination_thr == NULL ? _LC_DEFAULT_INCLINATION_THR : *config->inclination_thr;
    lp->start_points_count = config == NULL || config->start_points_count == NULL ? _LC_DEFAULT_START_POINTS_COUNT : *config->start_points_count;
    lp->start_points = (lc_point_t*) malloc(sizeof(lc_point_t) * lp->start_points_count);
    
    return lp;
}

int counterPoints;
double ultimiPunti[_LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH][2];

int lc_eval_point(lc_counter_t *lp, lc_point_t *p, lc_counter_t *lp_inclination)
{
    // Bool result if new lap
    int new_lap = 0;

    // Sets the last two points (last and current) and updates the related vector (current_vector)
    _update_current_vector(lp, p);
    ultimiPunti[counterPoints%_LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH][0] = p->x;
    ultimiPunti[counterPoints%_LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH][1] = p->y;
    if (counterPoints >= _LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH) {
        lc_point_t *last_batchPoint = lc_point_init(ultimiPunti[(counterPoints+1)%_LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH][0], ultimiPunti[(counterPoints+1)%_LC_NUMBER_OF_POINTS_FOR_INCLINATION_BATCH][1]);
        _update_current_vector(lp_inclination, last_batchPoint);
    }

    // If the point is one of the firsts, use it to create the start line
    if (lp->start_point_index < lp->start_points_count)
    {
        _update_startline_points(lp, p);
    }
    // If the points for the start line are just finished, calculate the start line and the start vector
    else if (lp->start_point_index == lp->start_points_count)
    {
        _calc_startline(lp, p);
        lp_inclination->start_vector_angle = lp->start_vector_angle;
        lp_inclination->inclination_thr = lp->inclination_thr;
    }
    // Otherwise check if it is a new lap
    else
    {
        // Updates the proximity thr (if lap hasn't been found yet)
        if (lp->laps_count == 0)
            _update_proximity_thr(lp);

        // Reset last result
        _reset_last_results(lp);
        _reset_last_results(lp_inclination);

        int proxim = _check_proximity(lp);
        int inclin = _check_inclination(lp_inclination);
        //int inclin = _check_inclination(lp);
        int overl = _check_overlap(lp);
        fprintf(stderr, "%d: %d, %d, %d\n", counterPoints, proxim, inclin, overl);
        // If new lap, increment laps count and set result to true
        if (proxim && inclin && overl)
        {
            lp->laps_count++;
            new_lap = 1;
        }
    }
    counterPoints++;

    return new_lap;
}

void lc_reset(lc_counter_t *lp)
{
    lp->start_point_index = 0;
    lp->laps_count = 0;
    lp->proximity_thr = 0;
}

void lc_destroy(lc_counter_t *lp)
{
    free(lp->start_points);
    free(lp);
}

/** HELPER FUNCTIONS DEFINITIONS **/

static void _update_current_vector(lc_counter_t *lp, lc_point_t *p)
{
    lp->last_point = lp->current_point;
    lp->current_point = *p;
    lc_vector_set(&lp->current_vector, &lp->last_point, &lp->current_point);
}

static void _update_startline_points(lc_counter_t *lp, lc_point_t *p)
{
    lp->start_points[lp->start_point_index] = *p;
    ++lp->start_point_index;
}

static void _calc_startline(lc_counter_t *lp, lc_point_t *p)
{
    double avg_start_angle = 0;
    lc_vector_t temp_vect;

    /*
    // For every couple of start points
    for (int i = 0; i < lp->start_points_count - 1; ++i)
    {
        // Create a vector between the two points and add its angle to the avg_start_angle variable
        lc_vector_set(&temp_vect, &lp->start_points[i], &lp->start_points[i + 1]);
        avg_start_angle += lc_vector_angle(&temp_vect);
    }
    // Finish calculating the avg_start_angle
    avg_start_angle /= lp->start_points_count - 1;
    */

    lc_vector_set(&temp_vect, &lp->start_points[0], &lp->start_points[lp->start_points_count-1]);
    avg_start_angle = lc_vector_angle(&temp_vect);

    // Sets the start vector as a vector starting from the first point and having the average angle between the first points
    lc_vector_set_from_angle(&lp->start_vector, &lp->start_points[0], avg_start_angle);
    // Updates the start_vector_angle
    lp->start_vector_angle = avg_start_angle;
    // Sets the start line as the vector perpendicular to the start vector
    lc_vector_set_from_perpendicular(&lp->start_line, &lp->start_vector);
    // Updates the start_point_index
    lp->start_point_index++;
}

static void _update_proximity_thr(lc_counter_t *lp)
{
    double proximity_temp = lc_vector_length(&lp->current_vector) * lp->proximity_incr;
    if (proximity_temp > lp->proximity_thr)
    {
        lp->proximity_thr = proximity_temp;
    }
}

static void _reset_last_results(lc_counter_t *lp)
{
    lp->last_proximity_result = 0;
    lp->last_inclination_result = 0;
    lp->last_overlap_result = 0;
}

static int _check_proximity(lc_counter_t *lp)
{
    double d1 = lc_point_distance(&lp->last_point, &lp->start_points[0]);
    double d2 = lc_point_distance(&lp->current_point, &lp->start_points[0]);
    int result = d1 < lp->proximity_thr && d2 < lp->proximity_thr;
    fprintf(stderr, "%lf, %lf | ", d1, lp->proximity_thr);
    lp->last_proximity_result = result;
    return result;
}

static int _check_inclination(lc_counter_t *lp)
{
    double current_vector_angle = lc_vector_angle(&lp->current_vector);
    double angle_difference = (double)((int)(current_vector_angle - lp->start_vector_angle + 180) % 360 - 180);
    //double angle_difference = (double)((int)current_vector_angle - lp->start_vector_angle);
    int result = fabs(angle_difference) <= lp->inclination_thr;
    //fprintf(stderr, "%lf %lf | ", current_vector_angle, lp->start_vector_angle);
    lp->last_inclination_result = result;
    return result;
}

static int _check_overlap(lc_counter_t *lp)
{
    lc_vector_t v1, v2;
    int overlap_p1cond, overlap_p2cond;
    lc_vector_set(&v1, &lp->start_line.p1, &lp->current_vector.p1);
    lc_vector_set(&v2, &lp->start_line.p1, &lp->current_vector.p2);
    overlap_p1cond = lc_vector_det(&lp->start_line, &v1) > 0;
    overlap_p2cond = lc_vector_det(&lp->start_line, &v2) > 0;
    int result = overlap_p1cond != overlap_p2cond;
    lp->last_overlap_result = result;
    return result;
}
