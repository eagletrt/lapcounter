%module lapcounter

%{
#include "lapcounter/lapcounter.h"
%}

extern int lc_do_stuff();
extern lc_counter_t *lc_init(lc_counter_config_t *config);
extern int lc_eval_point(lc_counter_t *lp, lc_point_t *p, lc_counter_t *lp_inclination);
extern void lc_reset(lc_counter_t *lp);
extern void lc_destroy(lc_counter_t *lp);

%include "cpointer.i"
%pointer_functions(lc_counter_config_t, lc_counter_config)
%pointer_functions(lc_counter_t, lc_counter)
%pointer_functions(lc_point_t, lc_point)