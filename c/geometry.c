#include <math.h>
#include <stdio.h>
#include "geometry.h"

//#define M_PI acos(-1.0)
#define TO_DEG 180 / M_PI
#define TO_RAD M_PI / 180

void set_vector(Vector * v_ptr, Point * p1, Point * p2) {
    v_ptr->p1 = *p1;
    v_ptr->p2 = *p2;
    v_ptr->dx = p2->x - p1->x;
    v_ptr->dy = p2->y - p1->y;
}

void set_vector_from_angle(Vector * v_ptr, Point * p1, float angle) {
    v_ptr->p1 = *p1;
    v_ptr->dx = cos(angle * TO_RAD);
    v_ptr->dy = sin(angle * TO_RAD);
    v_ptr->p2.x = p1->x + v_ptr->dx;
    v_ptr->p2.y = p1->y + v_ptr->dy;
}

void get_vector_axis(Vector * v_ptr, char c, double len) {
    Point p1 = {0, 0};
    Point p2;
    if (c == 'x') {
        p2 = (Point){len, 0};
    } else if (c == 'y') {
        p2 = (Point){0, len};
    }
    set_vector(v_ptr, &p1, &p2);
}

void get_perpendicular_vector(Vector * vin_ptr, Vector * vout_ptr) {
    double multiplier = 1 / get_vector_length(vin_ptr) * 10;
    double x1 = vin_ptr->p1.x + vin_ptr->dy * multiplier;
    double y1 = vin_ptr->p1.y - vin_ptr->dx * multiplier;
    Point p1 = {x1, y1};
    double x2 = vin_ptr->p1.x - vin_ptr->dy * multiplier;
    double y2 = vin_ptr->p1.y + vin_ptr->dx * multiplier;
    Point p2 = {x2, y2};
    set_vector(vout_ptr, &p1, &p2);
}

double get_vector_length(Vector * v) {
    return sqrt(pow(v->dx,2) + pow(v->dy, 2));
}

double get_vector_angle(Vector * v) {
    //Vector axis;
    //get_vector_axis(&axis, 'x', 1);
    //double det = determinant(v, axis);
    //double dot = dot_product(v, axis);
    //double radians = atan2(det, dot);
    //return radians * 180 / M_PI;
    return atan2(v->dx, v->dy) * TO_DEG;
}

double dot_product(Vector * v1, Vector * v2) {
    return v2->dx * v1->dx + v2->dy * v1->dy;
}

double determinant(Vector * v1, Vector * v2) {
    return v1->dx * v2->dy - v1->dy * v2->dx;
}