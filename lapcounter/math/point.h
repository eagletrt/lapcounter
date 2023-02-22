#ifndef _LC_POINT_H
#define _LC_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

/**
 * @brief The type of a point in the cartesian plane
 */
typedef struct {
  /** Coordinates in the x axis */
  double x;
  /** Coordinates in the y axis */
  double y;
} lc_point_t;

typedef lc_point_t lc_vector_t;

/**
 * @brief Returns the distance between two points
 *
 * @param p1 The ref to the first point
 * @param p2 The ref to the second point
 * @returns The string that represents the point
 */
double lc_point_distance(const lc_point_t *p1, const lc_point_t *p2);


/**
 * @brief Updates the values of the passed vector with the given vertexes.
 *
 * @param v The reference to the vector to update
 * @param p The reference to the first vertex.
 */
void lc_vector_set(lc_vector_t *v, const lc_point_t *p);

/**
 * @brief Updates the values of the passed vector given a vertex and an angle
 *
 * @param v The reference to the vector to update
 * @param angle The angle
 */
void lc_vector_set_from_angle(lc_vector_t *v, double angle);

/**
 * @brief Updates the given vector given its perpendicular.
 *
 * @param v The ref to the vector that will be updated
 * @param vp The ref to the perpendicular vector
 */
void lc_vector_set_from_perpendicular(lc_vector_t *v, const lc_vector_t *vp);

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
 * @brief Return the dot product between two vectors
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @returns The dot product of the two vectors.
 */
double lc_vector_dot(const lc_vector_t *v1, const lc_vector_t *v2);

/**
 * @brief Return the determinant of the two vectors
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 * @returns The determinant of the two vectors.
 */
double lc_vector_det(const lc_vector_t *v1, const lc_vector_t *v2);

#ifdef __cplusplus
}
#endif

#endif