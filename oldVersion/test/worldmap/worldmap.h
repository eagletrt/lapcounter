#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../../src/lapcounter.h"

void worldmap(lc_counter_t * lp, char *path);

#endif