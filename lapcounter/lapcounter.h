#ifndef _LC_LAPCOUNTER_H
#define _LC_LAPCOUNTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stddef.h>

#include "math/point.h"

#define LC_DEFAULT_PROXIMITY_INCREMENT 1.85
#define LC_DEFAULT_INCLINATION_THRESHOLD 0.8
#define LC_DEFAULT_DISTANCE_THRESHOLD 0.000001
#define LC_DEFAULT_START_POINTS_COUNT 10

/**
 * @brief The struct containing the lap info
 */
typedef struct _lc_counter_t {
  /** The first points, used to determine the start line and vector **/
  lc_point_t start_points[2];
  /** The number of first points that will be used to calculate the first line **/
  size_t start_points_count;
  /** The index of the current point, used to understand if the start line is to be evaluated and incremented only in
   * the beginning **/
  size_t start_point_index;

  /** The start vector, a vector that starts from the first point and has as direction the average angle of the first
   * points' vectors **/
  lc_vector_t start_inclination_vector;
  /** The start line, vector perpendicular to the start vector **/
  lc_vector_t start_line_vector;

  /** The current vector (between last and current point) **/
  lc_vector_t current_vector;

  lc_point_t current_point;
  lc_point_t last_point;

  /** Last result of proximity evaluation **/
  int last_proximity_result;
  /** Last result of inclination evaluation **/
  int last_inclination_result;
  /** Last result of overlap evaluation **/
  int last_overlap_result;

  double proximity_threshold;
  double proximity_increment;
  double inclination_threshold;
  double distance_threshold;

  /** The number of laps that have been detected **/
  int laps_count;
} lc_counter_t;

/**
 * @brief Creates a leap counter
 *
 * @param config The configurations for the leap counter.
 * @return The created leap counter
 */
lc_counter_t *lc_init(double proximity_increment, double inclination_threshold, double distance_threshold,
                      size_t start_points_count);

/**
 * @brief Adds a point to the leap counter and returns if there is a new leap
 *
 * @param counter The leap counter reference.
 * @param p The new point reference.
 * @return int 1 if there is a new leap
 */
int lc_eval_point(lc_counter_t *counter, lc_point_t *point);

/**
 * @brief Resets the leap counter, as it were after the init function
 *
 * @param counter The ref to the leap counter
 */
void lc_reset(lc_counter_t *counter);

/**
 * @brief Deallocates a leap counter
 *
 * @param counter The leap counter to deallocate
 */
void lc_destroy(lc_counter_t *counter);

#ifdef __cplusplus
}
#endif

#endif
