#include "./worldmap.h"

#define RED "f00"
#define YELLOW "ff0"
#define GREEN "0f0"
#define BLUE "00f"
#define WHITE "fff"
#define BLACK "000"
#define MAX 20

void worldmap(lc_counter_t * lp, char *path) {

    usleep(25*1000);

    char tt[MAX], slat[MAX], slon[MAX], sangle[MAX];

    sprintf(tt, "P:%d,I:%d,O:%d", lp->last_proximity_result, lp->last_inclination_result, lp->last_overlap_result);
    sprintf(slat, "%4.8f", lp->current_point.x);
    sprintf(slon, "%4.8f", lp->current_point.y);
    sprintf(sangle, "%4.8f", lc_vector_angle(&lp->current_vector));

    char color[4];
    int results = lp->last_proximity_result + lp->last_inclination_result + lp->last_overlap_result;
    switch (results) {
        case 0: strcpy(color, WHITE); break;
        case 1: strcpy(color, GREEN); break;
        case 2: strcpy(color, YELLOW); break;
        case 3: strcpy(color, RED); break;
        default: strcpy(color, BLACK); break;
    }

    int child = fork() == 0;
    if (child) {
        execl("/bin/sh", "sh", path, slat, slon, sangle, tt, color, NULL);
        perror("aaaaa");
    } else {
        while(wait(NULL)>0);
    }
    
}

