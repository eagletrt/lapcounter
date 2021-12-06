#ifndef _LC_POINT_H
#define _LC_POINT_H

/** INCLUDES **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** STRUCTS **/

/**
 * @brief The type of a point in the cartesian plane
 */
typedef struct _lc_point_t
{
    /** Coordinates in the x axis */
    double x;
    /** Coordinates in the y axis */
    double y;
} lc_point_t;

/** FUNCTIONS **/

/**
 * @brief Allocates a point with the given coordinates
 * 
 * @param x The x coordinate
 * @param y The y coordinate
 * @returns The reference to the created point
 */
lc_point_t* lc_point_init(double x, double y);

/**
 * @brief Returns the distance between two points
 * 
 * @param p1 The ref to the first point
 * @param p2 The ref to the second point
 * @returns The string that represents the point
 */
double lc_point_distance(const lc_point_t *p1, const lc_point_t *p2);

/**
 * @brief Returns a string representing the given point
 * 
 * @param p The ref to the point that will be represented
 * @returns The string that represents the point
 */
char* lc_point_to_str(const lc_point_t *p);

/**
 * @brief Prints a point to the stdout
 * 
 * @param p The ref to the point to print
 */
void lc_point_print(const lc_point_t *p);


#endif