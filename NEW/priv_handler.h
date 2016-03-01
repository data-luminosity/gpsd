#ifndef PRIV_HANDLER_H
#define PRIV_HANDLER_H
#include <time.h>
#include <stdbool.h>
#include "app_entry.h"
//TODO include gps_fix_t


struct gps_fix_t {
    //NOTE: this time is time in which gps updates, not time of last request
    //timestamp_t time;	/* Time of update */
    int    mode;	/* Mode of fix */
#define MODE_NOT_SEEN	0	/* mode update not seen yet */
#define MODE_NO_FIX	1	/* none */
#define MODE_2D  	2	/* good for latitude/longitude */
#define MODE_3D  	3	/* good for altitude/climb too */
    double ept;		/* Expected time uncertainty */
    double latitude;	/* Latitude in degrees (valid if mode >= 2) */
    double epy;  	/* Latitude position uncertainty, meters */
    double longitude;	/* Longitude in degrees (valid if mode >= 2) */
    double epx;  	/* Longitude position uncertainty, meters */
    double altitude;	/* Altitude in meters (valid if mode == 3) */
    double epv;  	/* Vertical position uncertainty, meters */
    double track;	/* Course made good (relative to true north) */
    double epd;		/* Track uncertainty, degrees */
    double speed;	/* Speed over ground, meters/sec */
    double eps;		/* Speed uncertainty, meters/sec */
    double climb;       /* Vertical speed, meters/sec */
    double epc;		/* Vertical speed uncertainty */
};

//time structure in C's time.h
typedef struct timeval timeval_t;

//https://www.mpp.mpg.de/~huber/util/timevaldiff.c
//returns difference in time in milliseconds
int timediffval(timeval_t* start, timeval_t* end);


#define EPS_GRID_TYPE 0
#define EPS_RAD_TYPE 1

//returns a modified gps_data_t to write to client
//will handle epoch, type, and epsilon
struct gps_fix_t* gps_data_modify(gps_priv_t*, 
        struct gps_fix_t* src, struct gps_fix_t* dest);

//it would be in handle request before
bool gps_epoch_allow_update(int epoch, timeval_t* prev);
struct gps_fix_t* gps_epsilon_modifyGRID(int eps, 
        struct gps_fix_t* src, struct gps_fix_t* dest);
struct gps_fix_t* gps_epsilon_modifyRAD(int eps,
        struct gps_fix_t* src, struct gps_fix_t* dest);

#endif
