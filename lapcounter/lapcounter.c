#include "lapcounter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Updates the current vector by adding a new point
static void _update_current_vector(lc_counter_t *counter, lc_point_t *point);
// Updates the startline points by adding a new point
static void _update_startline_points(lc_counter_t *counter, lc_point_t *point);
// Calculates the start line and the start vector, used when the start points are all gathered
static void _calc_startline(lc_counter_t *counter);
static void _calc_sector_line(lc_counter_t *counter, lc_point_t *current_point);
// Resets to False (0) the results from the last evaluation
static void _reset_last_results(lc_counter_t *counter);

// Checks if the last point is very near to the first point
static int _check_proximity(lc_counter_t *counter, lc_point_t *point);
static int _check_proximity_start(lc_counter_t *counter);
// Checks if the inclination of the last vector is ok with the initial vector
static int _check_inclination(lc_counter_t *counter, lc_vector_t *inclination_vector);
static int _check_inclination_start(lc_counter_t *counter);
// Checks if the start line has been passed
static int _check_overlap(lc_counter_t *counter, lc_vector_t *line_vector, lc_point_t *point);
static int _check_overlap_start(lc_counter_t *counter);

// Calculates the correction factor for the new lap
static void _calc_correction_factor(lc_counter_t* counter);
static void _calc_correction_factor_sector(lc_counter_t* counter);

static inline double dist(double x1, double y1, double x2, double y2)
{ return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));}


void _lc_init_sectors(lc_counter_t *counter, int sector_count){
  counter->points = (lc_point_t *)malloc(sizeof(lc_point_t)*sector_count);
  counter->sector_inclination_vector = (lc_vector_t *)malloc(sizeof(lc_vector_t)*sector_count);
  counter->sector_line_vector = (lc_vector_t *)malloc(sizeof(lc_vector_t)*sector_count);
  memset(counter->points, 0, sizeof(lc_point_t) * sector_count);
  memset(counter->sector_inclination_vector, 0, sizeof(lc_vector_t) * sector_count);
  memset(counter->sector_line_vector, 0, sizeof(lc_vector_t) * sector_count);
}

lc_counter_t *lc_init(double proximity_threshold, double inclination_threshold, double distance_threshold,
                      size_t start_points_count, int sector_count) {
  lc_counter_t *counter = (lc_counter_t *)malloc(sizeof(lc_counter_t));

  _lc_init_sectors(counter, sector_count);

  counter->start_point_index = 0;
  counter->laps_count = 0;
  counter->sector_index = 0;

  counter->sector_count = (sector_count<=0)?1:sector_count;
  counter->proximity_threshold = proximity_threshold;
  counter->inclination_threshold = inclination_threshold;
  counter->distance_threshold = distance_threshold;

  counter->start_points_count = start_points_count;
  memset(counter->start_points, 0, sizeof(lc_point_t) * 2);

  return counter;
}

int lc_eval_point(lc_counter_t *counter, lc_point_t *point) {
  int under_threshold = 0; // if the points are closer than threshold
  if (fabs(point->x - counter->current_point.x) < counter->distance_threshold &&
      fabs(point->y - counter->current_point.y) < counter->distance_threshold) {
    under_threshold = 1;
  }

  // if(under_threshold == 0 && counter->laps_count == 0 && counter->start_point_index > 0)

  // Sets the last two points (last and current) and updates the related vector (current_vector)
  _update_current_vector(counter, point);

  if(under_threshold == 0 && counter->start_point_index <= counter->start_points_count){
    if (counter->start_point_index == 0) {
      counter->start_points[0] = *point;
    } else if (counter->start_point_index == counter->start_points_count){
      counter->start_points[1] = *point;
      // If the points for the start line are just finished, calculate the start line and the start vector
      _calc_startline(counter);

      //the last sector point is the starting point
      counter->points[counter->sector_count-1] = counter->start_points[0];
      counter->sector_inclination_vector[counter->sector_count-1] = counter->start_inclination_vector;
      counter->sector_line_vector[counter->sector_count-1] = counter->start_line_vector;
    }
    counter->start_point_index ++;
  } else {
    // Reset last result
    _reset_last_results(counter);

    if(counter->sector_count > 1 && counter->sector_index != counter->sector_count-1)
    {
      int new_sector = _check_proximity(counter, &counter->points[counter->sector_index]) &&
            _check_inclination(counter, &counter->sector_inclination_vector[counter->sector_index]) &&
            _check_overlap(counter, &counter->sector_line_vector[counter->sector_index], &counter->points[counter->sector_index]);
      counter->sector_index += new_sector;
      if(new_sector)
      {
        _calc_correction_factor_sector(counter);
        return counter->sector_index;
      }
    }

    if(_check_proximity_start(counter) == 0)
      return -1;
    if(_check_inclination_start(counter) == 0)
      return -1;
    if(_check_overlap_start(counter) == 0)
      return -1;

    // if new lap, increment laps count and set result to true
    counter->laps_count++;
    counter->sector_index = 0;
    _calc_correction_factor(counter);
    return 0;
  }

  return -1;
}

void lc_add_sector(lc_counter_t *counter, lc_point_t *point) {
  if(counter->sector_index < counter->sector_count-1)
  {
    counter->points[counter->sector_index] = *point;
    _calc_sector_line(counter, point);
  }
}

void lc_reset(lc_counter_t *counter) {
  _reset_last_results(counter);

  counter->start_point_index = 0;
  counter->laps_count = 0;
  // counter->proximity_threshold = 0;

  counter->start_line_vector.x = 0;
  counter->start_line_vector.y = 0;
  counter->start_inclination_vector.x = 0;
  counter->start_inclination_vector.y = 0;
  counter->current_vector.x = 0;
  counter->current_vector.y = 0;
  counter->sector_index = 0;

  memset(counter->start_points, 0, sizeof(lc_point_t) * 2);
  memset(counter->points, 0, sizeof(lc_point_t) * counter->sector_count);
  memset(counter->sector_inclination_vector, 0, sizeof(lc_vector_t) * counter->sector_count);
  memset(counter->sector_line_vector, 0, sizeof(lc_vector_t) * counter->sector_count);
}

void lc_destroy(lc_counter_t *counter) {
  free(counter);
}

static void _update_current_vector(lc_counter_t *counter, lc_point_t *point) {
  counter->last_point = counter->current_point;
  counter->current_point = *point;

  counter->current_vector.x = counter->last_point.x - counter->current_point.x;
  counter->current_vector.y = counter->last_point.y - counter->current_point.y;
}

static void _calc_sector_line(lc_counter_t *counter, lc_point_t *current_point) {
  lc_vector_t vector;
  double angle = 0;

  vector.x = counter->last_point.x - current_point->x;
  vector.y = counter->last_point.y - current_point->y;

  angle = lc_vector_angle(&vector);

  // Sets the start vector as a vector starting from the first point and having the average angle between the first
  // points
  lc_vector_set_from_angle(&counter->sector_inclination_vector[counter->sector_index], angle);

  lc_vector_set_from_perpendicular(&counter->sector_line_vector[counter->sector_index], &counter->sector_inclination_vector[counter->sector_index]);
}

static void _calc_startline(lc_counter_t *counter) {
  lc_vector_t vector;
  double angle = 0;

  vector.x = counter->start_points[0].x - counter->start_points[1].x;
  vector.y = counter->start_points[0].y - counter->start_points[1].y;
  angle = lc_vector_angle(&vector);

  // Sets the start vector as a vector starting from the first point and having the average angle between the first
  // points
  lc_vector_set_from_angle(&counter->start_inclination_vector, angle);

  lc_vector_set_from_perpendicular(&counter->start_line_vector, &counter->start_inclination_vector);

  // Complete start point estimation
  counter->start_point_index++;
}


static void _reset_last_results(lc_counter_t *counter) {
  counter->last_proximity_result = 0;
  counter->last_inclination_result = 0;
  counter->last_overlap_result = 0;
}

static int _check_proximity_start(lc_counter_t *counter){
  counter->last_proximity_result = _check_proximity(counter, &counter->start_points[0]);
  return counter->last_proximity_result;
}

static int _check_proximity(lc_counter_t *counter, lc_point_t *point) {
  lc_vector_t travel = {counter->current_point.x - counter->last_point.x,
                        counter->current_point.y - counter->last_point.y};
  
  if(travel.x == 0 && travel.y == 0)
    return 0;

  double distance = lc_point_line_distance(point, &counter->last_point, &counter->current_point);

  return distance < counter->proximity_threshold;
}

static int _check_inclination_start(lc_counter_t *counter){
  counter->last_inclination_result = _check_inclination(counter, &counter->start_inclination_vector);
  return counter->last_inclination_result;
}

static int _check_inclination(lc_counter_t *counter, lc_vector_t *inclination_vector) {
  double current_angle = lc_vector_angle(&counter->current_vector);
  double start_angle = lc_vector_angle(inclination_vector);

  double angle_difference = fabs(current_angle - start_angle);
  if (angle_difference < 0) {
    angle_difference += 2 * M_PI;
  }

  return angle_difference <= counter->inclination_threshold;
}

static int _check_overlap_start(lc_counter_t *counter) {
  return counter->last_overlap_result = _check_overlap(counter, 
                                    &counter->start_line_vector,
                                    &counter->start_points[0]);
}

static int _check_overlap(lc_counter_t *counter, lc_vector_t *line_vector, lc_point_t *point) {
  int overlap_p1cond, overlap_p2cond;

  //lc_point_t start_point = counter->start_points[0];

  lc_vector_t v1 = (lc_vector_t){counter->current_point.x - point->x, counter->current_point.y - point->y};
  lc_vector_t v2 = (lc_vector_t){counter->last_point.x - point->x, counter->last_point.y - point->y};

  overlap_p1cond = lc_vector_det(line_vector, &v1) > 0;
  overlap_p2cond = lc_vector_det(line_vector, &v2) > 0;

  return overlap_p1cond != overlap_p2cond;
}

static void _calc_correction_factor_sector(lc_counter_t* counter) {
  int ind = counter->sector_index-1;
  if(ind < 0) ind = counter->sector_count-1;
  double min_distance = lc_point_line_distance(&counter->points[ind], &counter->last_point, &counter->current_point);
  double last_distance = lc_point_distance(&counter->points[ind], &counter->last_point);
  double travel_distance = lc_point_distance(&counter->last_point, &counter->current_point);

  double intersection_distance = sqrt(pow(last_distance, 2) - pow(min_distance, 2));

  counter->correction_factor_sector = intersection_distance / travel_distance;
}

static void _calc_correction_factor(lc_counter_t* counter){
  double min_distance = lc_point_line_distance(&counter->start_points[0], &counter->last_point, &counter->current_point);
  double last_distance = lc_point_distance(&counter->start_points[0], &counter->last_point);
  double travel_distance = lc_point_distance(&counter->last_point, &counter->current_point);

  double intersection_distance = sqrt(pow(last_distance, 2) - pow(min_distance, 2));

  counter->correction_factor = intersection_distance / travel_distance;
}

int lc_save(lc_counter_t* counter, char* path){
  FILE *f = fopen(path, "wb");
  if(f == NULL) return 1;
  fwrite(&counter->sector_count, 8, 1, f);
  for(int i=0;i<counter->sector_count;i++)
  {
    fwrite(&counter->points[i].x, 8, 1, f);
    fwrite(&counter->points[i].y, 8, 1, f);
    fwrite(&counter->sector_inclination_vector[i].x, 8, 1, f);
    fwrite(&counter->sector_inclination_vector[i].y, 8, 1, f);
    fwrite(&counter->sector_line_vector[i].x, 8, 1, f);
    fwrite(&counter->sector_line_vector[i].y, 8, 1, f);
  }
  fclose(f);
  return 0;
}

int lc_load(lc_counter_t* counter, char* path){
  FILE *f = fopen(path, "r");
  if(f == NULL) return 1;
  double tmp;
  fread(&counter->sector_count, 8, 1, f);
  _lc_init_sectors(counter, counter->sector_count);
  for(int i=0;i<counter->sector_count;i++)
  {
    fread(&counter->points[i].x, 8, 1, f);
    fread(&counter->points[i].y, 8, 1, f);
    fread(&counter->sector_inclination_vector[i].x, 8, 1, f);
    fread(&counter->sector_inclination_vector[i].y, 8, 1, f);
    fread(&counter->sector_line_vector[i].x, 8, 1, f);
    fread(&counter->sector_line_vector[i].y, 8, 1, f);
  }
  fclose(f);
  return 0;
}