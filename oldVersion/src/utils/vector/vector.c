#include "vector.h"

void lc_vector_set(lc_vector_t *v, const lc_point_t *p1, const lc_point_t *p2)
{
    v->p1 = *p1;
    v->p2 = *p2;
    v->dx = p2->x - p1->x;
    v->dy = p2->y - p1->y;
}

void lc_vector_set_from_angle(lc_vector_t *v, const lc_point_t *p1, double angle)
{
    v->p1 = *p1;
    v->dx = cos(angle * TO_RAD);
    v->dy = sin(angle * TO_RAD);
    v->p2.x = p1->x + v->dx;
    v->p2.y = p1->y + v->dy;
}

void lc_vector_set_from_versor(lc_vector_t *v, char axis, double length)
{
    lc_point_t p1 = {0, 0};
    lc_point_t p2;

    switch (axis)
    {
    case 'x':
        p2 = (lc_point_t){length, 0};
        break;
    case 'y':
        p2 = (lc_point_t){0, length};
        break;
    }

    lc_vector_set(v, &p1, &p2);
}

lc_vector_t *lc_vector_init(const lc_point_t *p1, const lc_point_t *p2)
{
    lc_vector_t *result = (lc_vector_t *)malloc(sizeof(lc_vector_t));
    lc_vector_set(result, p1, p2);
}

lc_vector_t *lc_vector_init_from_angle(const lc_point_t *p, double angle)
{
    lc_vector_t *result = (lc_vector_t *)malloc(sizeof(lc_vector_t));
    lc_vector_set_from_angle(result, p, angle);
}

lc_vector_t *lc_vector_init_from_versor(char axis, double length)
{
    lc_vector_t *result = (lc_vector_t *)malloc(sizeof(lc_vector_t));
    lc_vector_set_from_versor(result, axis, length);
}

lc_vector_t *lc_vector_init_from_perpendicular(const lc_vector_t *v) {
    lc_vector_t *result = (lc_vector_t *)malloc(sizeof(lc_vector_t));
    lc_vector_set_from_perpendicular(result, v);
}

double lc_vector_length(const lc_vector_t *v)
{
    return lc_point_distance(&(v->p1), &(v->p2));
}

double lc_vector_angle(const lc_vector_t *v)
{
    return atan2(v->dx, v->dy) * TO_DEG;
}

void lc_vector_set_from_perpendicular(lc_vector_t *v, const lc_vector_t *vp)
{
    double multiplier = 1 / lc_vector_length(vp) * 50;

    double x1 = vp->p1.x + vp->dy * multiplier;
    double y1 = vp->p1.y - vp->dx * multiplier;
    lc_point_t p1 = {x1, y1};

    double x2 = vp->p1.x - vp->dy * multiplier;
    double y2 = vp->p1.y + vp->dx * multiplier;
    lc_point_t p2 = {x2, y2};

    lc_vector_set(v, &p1, &p2);
}

char *lc_vector_to_str(const lc_vector_t *v)
{
    char* p1_str = lc_point_to_str(&v->p1);
    char* p2_str = lc_point_to_str(&v->p2);
    double angle = lc_vector_angle(v);

    int p1_len = strlen(p1_str);
    int p2_len = strlen(p2_str);
    int dx_len = snprintf(NULL, 0, "%f", v->dx);
    int dy_len = snprintf(NULL, 0, "%f", v->dy);
    int angle_len = snprintf(NULL, 0, "%f", angle);

    int res_len = 13 + p1_len + p2_len + dx_len + dy_len + angle_len + 1;

    char *result = (char *)malloc(res_len * sizeof(char));
    sprintf(result, "vector(%s,%s,%f,%f,%f°)", p1_str, p2_str, v->dx, v->dy, angle);

    return result;
}

void lc_print_vector(const lc_vector_t *v)
{
    char *str = lc_vector_to_str(v);
    printf("%s\n", str);
    free(str);
}

double lc_vector_prod(const lc_vector_t *v1, const lc_vector_t *v2) {
    return v2->dx * v1->dx + v2->dy * v1->dy;
}

double lc_vector_det(const lc_vector_t *v1, const lc_vector_t *v2) {
    return v1->dx * v2->dy - v1->dy * v2->dx;
}