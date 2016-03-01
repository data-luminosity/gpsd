#include "priv_handler.h"
#include <stdlib.h>
#include <math.h>
int timediffval(timeval_t* start, timeval_t* end);


#define RADIUS_TYPE 0

#define GRID_TYPE 1
//returns a modified struct gps_fix_t to write to client
//will handle epoch, type, and epsilon
struct gps_fix_t* gps_data_modify(gps_priv_t* settings, 
        struct gps_fix_t* src, struct gps_fix_t* dest){

    //memcpy
    //TODO error checking
    memcpy(dest,src, sizeof(gps_fix_t));
    
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


