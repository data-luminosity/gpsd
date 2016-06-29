#ifndef PRIV_HANDLER_H
#define PRIV_HANDLER_H
#include <time.h>
#include <stdbool.h>
#include "app_entry.h"
#include "gps.h"


//time structure in C's time.h
typedef struct timeval timeval_t;

//https://www.mpp.mpg.de/~huber/util/timevaldiff.c
//returns difference in time in milliseconds
long timediffval(timeval_t* start, timeval_t* end);


#define EPS_GRID_TYPE 0
#define EPS_RAD_TYPE 1
#define MAX_LOC_VALS 10

#define RADIUS_TYPE 0
#define GRID_TYPE 1

typedef struct location{
    double longitude;
    double latitude;
} location_t;

//it would be in handle request before
bool gps_epoch_allow_update(long epoch, timeval_t* prev);
bool gps_data_modify(gps_priv_t*, location_t* src, location_t* dest, int* n_locs);
bool gps_data_modifyRAD(epsilon_t, location_t* src, location_t* dest, int* n_locs);
bool gps_data_modifyGRID(epsilon_t, location_t* src, location_t* dest, int* n_locs, double flipone, double fliptwo);
#endif
