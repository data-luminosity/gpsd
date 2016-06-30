#include <stdlib.h>
#include <time.h>

/*
 * Copyright (c) 2005 Jeff Francis <jeff@gritch.org>
 * BSD terms apply: see the filr COPYING in the distribution root for details.
 */

/*
   Jeff Francis
   jeff@gritch.org

   Kind of a curses version of xgps for use with gpsd.
   */

/*
 * The True North compass fails with current gpsd versions for reasons
 * the dev team has been unable to diagnose due to not having test hardware.
 * The support for it is conditioned out in order to simplify moving
 * to the new JSON-based protocol and reduce startup time.
 */
#undef TRUENORTH

/* ==================================================================
   These #defines should be modified if changing the number of fields
   to be displayed.
   ================================================================== */

/* This defines how much overhead is contained in the 'datawin' window
   (eg, box around the window takes two lines). */
#define DATAWIN_OVERHEAD 2

/* This defines how much overhead is contained in the 'satellites'
   window (eg, box around the window takes two lines, plus the column
   headers take another line). */
#define SATWIN_OVERHEAD 3

/* This is how many display fields are output in the 'datawin' window
   when in GPS mode.  Change this value if you add or remove fields
   from the 'datawin' window for the GPS mode. */
#define DATAWIN_GPS_FIELDS 8

/* Count of optional fields that we'll display if we have the room. */
#define DATAWIN_OPTIONAL_FIELDS 7

/* This is how many display fields are output in the 'datawin' window
   when in COMPASS mode.  Change this value if you add or remove fields
   from the 'datawin' window for the COMPASS mode. */
#define DATAWIN_COMPASS_FIELDS 6

/* This is how far over in the 'datawin' window to indent the field
   descriptions. */
#define DATAWIN_DESC_OFFSET 5

/* This is how far over in the 'datawin' window to indent the field
   values. */
#define DATAWIN_VALUE_OFFSET 17

/* This is the width of the 'datawin' window.  It's recommended to
   keep DATAWIN_WIDTH + SATELLITES_WIDTH <= 80 so it'll fit on a
   "standard" 80x24 screen. */
#define DATAWIN_WIDTH 45

/* This is the width of the 'satellites' window.  It's recommended to
   keep DATAWIN_WIDTH + SATELLITES_WIDTH <= 80 so it'll fit on a
   "standard" 80x24 screen. */
#define SATELLITES_WIDTH 35

/* ================================================================
   You shouldn't have to modify any #define values below this line.
   ================================================================ */

/* This is the minimum size we'll accept for the 'datawin' window in
   GPS mode. */
#define MIN_GPS_DATAWIN_SIZE (DATAWIN_GPS_FIELDS + DATAWIN_OVERHEAD)

/* And the maximum size we'll try to use */
#define MAX_GPS_DATAWIN_SIZE (DATAWIN_GPS_FIELDS + DATAWIN_OPTIONAL_FIELDS + DATAWIN_OVERHEAD)

/* This is the minimum size we'll accept for the 'datawin' window in
   COMPASS mode. */
#define MIN_COMPASS_DATAWIN_SIZE (DATAWIN_COMPASS_FIELDS + DATAWIN_OVERHEAD)

/* This is the maximum number of satellites gpsd can track. */
#define MAX_POSSIBLE_SATS (MAXCHANNELS - 2)

/* This is the maximum size we need for the 'satellites' window. */
#define MAX_SATWIN_SIZE (MAX_POSSIBLE_SATS + SATWIN_OVERHEAD)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <curses.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

#include "gpsd_config.h"
#include "gps.h"
#include "compiler.h"	/* for UNUSED */
#include "gpsdclient.h"
#include "revision.h"
#include <time.h>
typedef struct timeval timeval_t;


static struct gps_data_t gpsdata;
static time_t status_timer;	/* Time of last state change. */
static int state = 0;		/* or MODE_NO_FIX=1, MODE_2D=2, MODE_3D=3 */
static float altfactor = METERS_TO_FEET;
static float speedfactor = MPS_TO_MPH;
static char *altunits = "ft";
static char *speedunits = "mph";
static struct fixsource_t source;
#ifdef CLIENTDEBUG_ENABLE
static int debug;
#endif /* CLIENTDEBUG_ENABLE */

static WINDOW *datawin, *satellites, *messages;

static bool raw_flag = false;
static bool silent_flag = false;
static bool magnetic_flag = false;
static int window_length;
static int display_sats;
#ifdef TRUENORTH
static bool compass_flag = false;
#endif /* TRUENORTH */

/* pseudo-signals indicating reason for termination */
#define CGPS_QUIT	0	/* voluntary yterminastion */
#define GPS_GONE	-1	/* GPS device went away */
#define GPS_ERROR	-2	/* low-level failure in GPS read */
#define GPS_TIMEOUT	-3	/* low-level failure in GPS waiting */

/* Function to call when we're all done.  Does a bit of clean-up. */
static void die(int sig)
{
    if (!isendwin())
    {
        /* Move the cursor to the bottom left corner. */
        (void)mvcur(0, COLS - 1, LINES - 1, 0);

        /* Put input attributes back the way they were. */
        (void)echo();

        /* Done with curses. */
        (void)endwin();
    }

    /* We're done talking to gpsd. */
    (void)gps_close(&gpsdata);

    switch (sig) {
        case CGPS_QUIT:
            break;
        case GPS_GONE:
            (void)fprintf(stderr, "cgps: GPS hung up.\n");
            break;
        case GPS_ERROR:
            (void)fprintf(stderr, "cgps: GPS read returned error\n");
            break;
        case GPS_TIMEOUT:
            (void)fprintf(stderr, "cgps: GPS timeout\n");
            break;
        default:
            (void)fprintf(stderr, "cgps: caught signal %d\n", sig);
            break;
    }

    /* Bye! */
    exit(EXIT_SUCCESS);
}

static enum deg_str_type deg_type = deg_dd;


long timediffval(timeval_t* start, timeval_t* end){
    return end->tv_sec - start->tv_sec;

    long end_ms = end->tv_sec*1000 + end->tv_usec/1000;
    long start_ms = start->tv_sec*1000 + start->tv_usec/1000;
    return end_ms - start_ms;
}

double act_lat = 34.0686259;
double act_long =-118.4433499118;
double tot_diff = 0;
double tot_count = 0;

double diff_squared(double lon, double lat){
    double diff_sq =  (act_lat - lat) * (act_lat- lat) + (act_long - lon)*(act_long - lon); 
    printf("double squared%f\n", diff_sq);
    if (diff_sq != diff_sq)
        return 0;
    return diff_sq;
}


void data_dump(struct gps_data_t * data, timeval_t* prev_time){
    timeval_t cur_time;
    gettimeofday(&cur_time, NULL);
    printf("time elapsed:%ld\tlongitude:%f\tlatitude%f\n", timediffval(prev_time, &cur_time), data->fix.longitude, data->fix.latitude);

    *prev_time = cur_time;
}

/*
 * No protocol dependencies above this line
 */

int main(int argc, char *argv[])
{
    unsigned int flags = WATCH_ENABLE;
    int wait_clicks = 0;  /* cycles to wait before gpsd timeout */

    int totalIterations;
    int appid; 
    //parsing arguments
    if (argc != 3){
        printf("ARG1: number of iterations, ARG2: appid\n"); 
        return -1;
    }
    totalIterations= atoi(argv[1]); 
    appid = atoi(argv[2]); 


    //getting time
    timeval_t curr_time;
    gettimeofday(&curr_time, NULL);

    /* Open the stream to gpsd. */
    if (gps_open(source.server, source.port, &gpsdata) != 0) {
        (void)fprintf(stderr,
                "cgps: no gpsd running or network error: %d, %s\n",
                errno, gps_errstr(errno));
        exit(EXIT_FAILURE);
    }

    status_timer = time(NULL);
    if (source.device != NULL)
        flags |= WATCH_DEVICE;
    (void)gps_stream(&gpsdata, flags, source.device);


    /* heart of the client */
    int nIteration = 0;
    
    //passing in app id
    gps_sock_pass_appid(&gpsdata,appid,source.device);
    
    //printing to a file
    printf("APP ID: %d\n", appid);
    while (nIteration <= totalIterations ) {
        int c;
        /* wait 1/2 second for gpsd */
        if (!gps_waiting(&gpsdata, 500000)) {
            /* 240 tries at .5 Sec a try is a 2 minute timeout */
            //printf("waiting\n");
            if ( 240 < wait_clicks++ )
                die(GPS_TIMEOUT);
        } else {
            wait_clicks = 0;
            errno = 0;
            if (gps_read(&gpsdata) == -1) {
                fprintf(stderr, "cgps: socket error 4\n");
                die(errno == 0 ? GPS_GONE : GPS_ERROR);
            } else{ 
                //dumping out values
                data_dump(&gpsdata,&curr_time);
                nIteration++;
            }
        }

    }
    die(0);
}
