#ifndef _LC_LAPCOUNTER_H
#define _LC_LAPCOUNTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stddef.h>

#include "math/point.h"

#define LC_DEFAULT_PROXIMITY_THRESHOLD 0.0001
#define LC_DEFAULT_INCLINATION_THRESHOLD 0.8
#define LC_DEFAULT_DISTANCE_THRESHOLD 0.000001
#define LC_DEFAULT_START_POINTS_COUNT 10
#define LC_DEFAULT_SECTOR_RANGE 0.0001

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

  int sector_count;
  int sector_index;
  lc_point_t *points;
  lc_vector_t *sector_inclination_vector;
  lc_vector_t *sector_line_vector;

  /** Last result of proximity evaluation **/
  int last_proximity_result;
  /** Last result of inclination evaluation **/
  int last_inclination_result;
  /** Last result of overlap evaluation **/
  int last_overlap_result;

  /** from 0 to 1 and defines how close is the start_line to the points of the new lap */
  /** sometimes the line is closer to current_point than to last_point and vice versa. */
  /** To get the exact point (and so the lap time) at which the line was crossed */
  /** the correction factor can be used: */
  /** x = x_prev + delta_x * correction_factor */
  /** y = y_prev + delta_y * correction_factor */
  /** the same can be done with the timestamp */
  double correction_factor;
  double correction_factor_sector;

  double proximity_threshold;
  double inclination_threshold;
  double distance_threshold;

  /** The number of laps that have been detected **/
  int laps_count;
} lc_counter_t;

/**
 * @brief Creates a leap counter
 *
 * @param config The configurations for the lap counter.
 * @return The created leap counter
 */
lc_counter_t *lc_init(double proximity_threshold, double inclination_threshold, double distance_threshold,
                      size_t start_points_count, int sector_count);

/**
 * @brief Saves sector positions and inclinations in binary format
*/
int lc_save(lc_counter_t* counter, char *path);
/**
 * @brief Loads the saved data
*/
int lc_load(lc_counter_t* counter, char *path);

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

/**
 * @brief Set current sector
 *
 * @param counter The lap counter 
 * @param point The point to set as end of sector
 */
void lc_add_sector(lc_counter_t *counter, lc_point_t *point);

/**
 * @brief Sets start line position without resetting the lap counter
 * 
 * @param counter The lap counter
 * @param point The new start line position 
*/
void lc_set_start_position(lc_counter_t *counter, lc_point_t *point);

/**
 * @brief Sets start line direction (in travel direction)
 * 
 * @param counter The lap counter
 * @param start_line The new inclination of the start line 
*/
void lc_set_start_inclination(lc_counter_t *counter, lc_vector_t *start_line);

/**
 * @brief Sets sector position
 * 
 * @param counter The lap counter
 * @param index The sector index
 * @param point The point to set as end of sector
 * @return 0 if the point is set, -1 if sector was not found
*/
int lc_set_sector_position(lc_counter_t *counter, int index, lc_point_t *point);

/**
 * @brief Sets sector line direction (in travel direction)
 * 
 * @param counter The lap counter
 * @param index The sector index
 * @param sector_line The direction vector of the sector line
 * @return 0 if the direction is set, -1 if sector was not found
*/
int lc_set_sector_inclination(lc_counter_t *counter, int index, lc_vector_t *sector_line);

#ifdef __cplusplus
}
#endif

#endif
