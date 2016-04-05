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

//returns a modified gps_data_t to write to client
//will handle epoch, type, and epsilon
struct gps_fix_t* gps_data_modify(gps_priv_t*, 
        struct gps_fix_t* src, struct gps_fix_t* dest);

//it would be in handle request before
bool gps_epoch_allow_update(long epoch, timeval_t* prev);
struct gps_fix_t* gps_epsilon_modifyGRID(int eps, 
        struct gps_fix_t* src, struct gps_fix_t* dest);
struct gps_fix_t* gps_epsilon_modifyRAD(int eps,
        struct gps_fix_t* src, struct gps_fix_t* dest);

#endif
