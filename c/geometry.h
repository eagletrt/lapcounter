#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point p1;
    Point p2;
    double dx;
    double dy;
} Vector;

void print_point(Point * p);
void print_vector(Vector * v);

void set_vector(Vector * v_ptr, Point * p1, Point * p2);

void set_vector_from_angle(Vector * v_ptr, Point * p1, float angle);

void get_vector_axis(Vector * v_ptr, char c, double len);

void get_perpendicular_vector(Vector * vin_ptr, Vector * vout_ptr);

double get_vector_length(Vector * v);

double get_vector_angle(Vector * v);

double dot_product(Vector * v1, Vector * v2);

double determinant(Vector * v1, Vector * v2);

double get_distance(Point * p1, Point * p2);

#endif