#include "point.h"

lc_point_t* lc_point_init(double x, double y) {
    lc_point_t *result = (lc_point_t*) malloc(sizeof(lc_point_t));
    result->x = x;
    result->y = y;
}

double lc_point_distance(const lc_point_t *p1, const lc_point_t *p2) {
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

char* lc_point_to_str(const lc_point_t *p) {
    int x_len = snprintf(NULL, 0, "%f", p->x);
    int y_len = snprintf(NULL, 0, "%f", p->y);
    int res_len = 8 + x_len + y_len + 1;

    char *result = (char*) malloc(res_len * sizeof(char));
    sprintf(result, "point(%f,%f)", p->x, p->y);
    
    return result;
}

void lc_print_point(const lc_point_t *p) {
    char *str = lc_point_to_str(p);
    printf("%s\n", str);
    free(str);
}
