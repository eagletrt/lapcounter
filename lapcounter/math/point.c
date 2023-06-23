#include "point.h"

#include <stdlib.h>

double lc_point_distance(const lc_point_t *p1, const lc_point_t *p2) {
  return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

double lc_point_line_distance(const lc_point_t *p, const lc_point_t *l1, const lc_point_t *l2){
  // dist = abs((x2-x1)*(y1-ys) - (x1-xs)*(y2-y1)) / sqrt((x2-x1)^2 + (y2-y1)^2)
  double num = fabs((l2->x - l1->x)*(l1->y - p->y ) - (l2->y - l1->y) * (l1->x - p->x ));
  double den = sqrt((l2->x - l1->x)*(l2->x - l1->x) + (l2->y - l1->y) * (l2->y - l1->y));
  return num / den;
}


void lc_vector_set(lc_vector_t *v, const lc_point_t *p) {
  v->x = p->x;
  v->y = p->y;
}

void lc_vector_set_from_angle(lc_vector_t *v, double angle) {
  v->x = cos(angle);
  v->y = sin(angle);
}

double lc_vector_length(const lc_vector_t *v) { return sqrt(pow(v->x, 2) + pow(v->y, 2)); }

double lc_vector_angle(const lc_vector_t *v) { return atan2(v->y, v->x); }

void lc_vector_set_from_perpendicular(lc_vector_t *v, const lc_vector_t *vp) {
  lc_vector_set(v, &(lc_point_t){vp->y, -vp->x});
}

double lc_vector_dot(const lc_vector_t *v1, const lc_vector_t *v2) { return v2->x * v1->x + v2->y * v1->y; }

double lc_vector_det(const lc_vector_t *v1, const lc_vector_t *v2) { return v1->x * v2->y - v1->y * v2->x; }