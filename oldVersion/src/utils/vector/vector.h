#ifndef _LC_VECTOR_H
#define _LC_VECTOR_H

/** INCLUDES **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./point/point.h"

/** DEFINES **/

#define TO_DEG 180 / M_PI
#define TO_RAD M_PI / 180

/** STRUCTS **/

/**
 * @brief The type of a vector in the cartesian plane, having two points and its direction.
 */
typedef struct _lc_vector_t
{
    /** The first vertex */
    lc_point_t p1;
    /** The second vertex */
    lc_point_t p2;
    /** The x projection */
    double dx;
    /** The y projection */
    double dy;
} lc_vector_t;

/** FUNCTIONS **/

/**
 * @brief Updates the values of the passed vector with the given vertexes.
 * 
 * @param v The reference to the vector to update
 * @param p1 The reference to the first vertex.
 * @param p2 The reference to the second vertex.
 */
void lc_vector_set(lc_vector_t *v, const lc_point_t *p1, const lc_point_t *p2);

/**
 * @brief Updates the values of the passed vector given a vertex and an angle
 * 
 * @param v The reference to the vector to update
 * @param p The reference to the vertex.
 * @param angle The angle
 */
void lc_vector_set_from_angle(lc_vector_t *v, const lc_point_t *p1, double angle);

/**
 * @brief Updates a vector given a versor (axis) and length.
 * 
 * @param v The reference to the vector to update
 * @param axis The axis 'x' or 'y'
 * @param length The length of the vector.
 */
void lc_vector_set_from_versor(lc_vector_t *v, char axis, double length);

/**
 * @brief Updates the given vector given its perpendicular.
 * 
 * @param v The ref to the vector that will be updated
 * @param vp The ref to the perpendicular vector
 */
void lc_vector_set_from_perpendicular(lc_vector_t *v, const lc_vector_t *vp);

/**
 * @brief Allocates a vector with the given points.
 * 
 * @param p1 The reference to the first vertex.
 * @param p2 The reference to the second vertex.
 * @returns The reference to the created vector
 */
lc_vector_t *lc_vector_init(const lc_point_t *p1, const lc_point_t *p2);

/**
 * @brief Allocates a vector given a vertex and an angle.
 * 
 * @param p The reference to the vertex.
 * @param angle The angle.
 * @returns The reference to the created vector
 */
lc_vector_t *lc_vector_init_from_angle(const lc_point_t *p1, double angle);

/**
 * @brief Allocates a vector given a versor (axis) and length.
 * 
 * @param axis The axis 'x' or 'y'
 * @param length The length of the vector.
 * @returns The reference to the created vector
 */
lc_vector_t *lc_vector_init_from_versor(char axis, double length);

/**
 * @brief Allocates a vector given its perpendicular vector.
 * 
 * @param v The perpendicular vector.
 * @returns The reference to the created vector
 */
lc_vector_t *lc_vector_init_from_perpendicular(const lc_vector_t *v);

/**
 * @brief Returns the vector's length
 * 
 * @param v The ref to the vector whose length will be calculated
 * @return The length of the vector 
 */
double lc_vector_length(const lc_vector_t *v);

/**
 * @brief Returns the vector's angle
 * 
 * @param v The ref to the vector whose angle will be calculated
 * @return The angle of the vector 
 */
double lc_vector_angle(const lc_vector_t *v);

/**
 * @brief Returns a string representing the given vector
 * 
 * @param v The vector that will be represented
 * @returns The string that represents the vector
 */
char *lc_vector_to_str(const lc_vector_t *v);

/**
 * @brief Prints a vector to the stdout
 * 
 * @param p The vector to print
 */
void lc_vector_print(const lc_vector_t *v);

/**
 * @brief Return the dot product between two vectors
 * 
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @returns The dot product of the two vectors.
 */
double lc_vector_prod(const lc_vector_t *v1, const lc_vector_t *v2);

/**
 * @brief Return the determinant of the two vectors
 * 
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @returns The determinant of the two vectors.
 */
double lc_vector_det(const lc_vector_t *v1, const lc_vector_t *v2);

#endif