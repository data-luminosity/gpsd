#include "priv_handler.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gps.h"
#include <stdio.h>



long timediffval(timeval_t* start, timeval_t* end){
    long end_ms = end->tv_sec*1000 + end->tv_usec/1000;
    long start_ms = start->tv_sec*1000 + start->tv_usec/1000;
    return end_ms - start_ms;
}

bool gps_epoch_allow_update(long epoch, timeval_t* prev){

    timeval_t curr_time;
    gettimeofday(&curr_time, NULL);

    printf("prev time:%ld\tcurr time:%ld\n", prev->tv_sec,curr_time.tv_sec);
    return timediffval(prev, &curr_time) > epoch;
}


bool gps_data_modify(gps_priv_t* settings, location_t* src, 
        location_t* dest, int* n_locs){
    switch(settings->type){
    case (RADIUS_TYPE):
        return gps_data_modifyRAD(settings->epsilon, src, dest, n_locs);
    case(GRID_TYPE):
        return gps_data_modifyGRID(settings->epsilon, src, dest, n_locs);
    default:
        return 1; //TODO not handled default case yet
    }
}

bool gps_data_modifyRAD(epsilon_t eps, location_t* src, location_t* dest, int* n_locs){
    if (src == NULL || dest == NULL)
        return 1;
    time_t t;
    srand((unsigned int) time(&t));

    //TODO not perfect distribution
    double dist_var = (double)(rand() % eps);
    int angle_var = rand();
    
    *n_locs = 1;
    dest[0].longitude = src->longitude + sin((double)angle_var)*dist_var;
    dest[1].latitude = src->latitude + cos((double)angle_var)*dist_var;
    return 1;
}

bool gps_data_modifyGRID(epsilon_t eps, location_t* src, location_t* dest, int* n_locs){
    return 1;
}
