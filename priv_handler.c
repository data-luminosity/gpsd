#include "priv_handler.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gps.h"

long timediffval(timeval_t* start, timeval_t* end){
    long end_ms = end->tv_sec*1000 + end->tv_usec/1000;
    long start_ms = start->tv_sec*1000 + start->tv_usec/1000;
    return end_ms - start_ms;



}

bool gps_epoch_allow_update(long epoch, timeval_t* prev){
    timeval_t curr_time;
    gettimeofday(&curr_time, NULL);
    return timediffval(prev, &curr_time) > epoch;
}

#define RADIUS_TYPE 0

#define GRID_TYPE 1
//returns a modified struct gps_fix_t to write to client
//will handle epoch, type, and epsilon
struct gps_fix_t* gps_data_modify(gps_priv_t* settings, 
        struct gps_fix_t* src, struct gps_fix_t* dest){
    //TODO error checking
    memcpy(dest,src, sizeof(struct gps_fix_t));
    
    switch (settings->type){
        case (RADIUS_TYPE):
                return gps_epsilon_modifyRAD(settings->epsilon, src, dest);
            break;
        case (GRID_TYPE):
                return gps_epsilon_modifyGRID(settings->epsilon, src, dest);
            break;
        default:
            return NULL;
            break;
    }
}

//TODO for simplifation, we will just do epsilon as some arbirary distance from
//and to actual longitude and latitude
struct gps_fix_t* gps_epsilon_modifyRAD(int eps,
        struct gps_fix_t* src, struct gps_fix_t* dest){
    if (src == NULL || dest == NULL)
        return NULL;
    time_t t;

    srand((unsigned int) time(&t));

    //TODO not perfect distribution
    double dist_var = (double)(rand() % eps);
    
    int angle_var = rand();
    

    dest->longitude  = src->longitude + sin((double)angle_var)*dist_var;
    dest->latitude  = src->latitude + cos((double)angle_var)*dist_var;
    return dest;
}

struct gps_fix_t* gps_epsilon_modifyGRID(int eps, 
        struct gps_fix_t* src, struct gps_fix_t* dest){
//TODO
return NULL;
}


