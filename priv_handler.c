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

    //printf("prev time:%ld\tcurr time:%ld\n", prev->tv_sec,curr_time.tv_sec);
    return timediffval(prev, &curr_time) > epoch;
}


bool gps_data_modify(gps_priv_t* settings, location_t* src, 
        location_t* dest, int* n_locs){
    switch(settings->type){
    case (RADIUS_TYPE):
        return gps_data_modifyRAD(settings->epsilon, src, dest, n_locs);
    case(GRID_TYPE):
        return gps_data_modifyGRID(settings->epsilon, src, dest, n_locs, settings->flipone, settings->fliptwo);
    default:
        return 1; //TODO not handled default case yet
    }
}


//RATIO
bool gps_data_modifyRAD(epsilon_t eps, location_t* src, location_t* dest, int* n_locs){
    if (src == NULL || dest == NULL)
        return 1;
    time_t t;
    srand((unsigned int) time(&t));

    //TODO not perfect distribution
    double dist_var = (double)(rand()/((double)RAND_MAX) * eps);
    int angle_var = rand();
    
    *n_locs = 1;
    printf("ORIGINAL LAT:%f\tLONG:%f\n", src->latitude, src->longitude);
    dest[0].longitude = src->longitude + sin((double)angle_var)*dist_var;
    dest[0].latitude = src->latitude + cos((double)angle_var)*dist_var;

    return 1;
}

bool flip(double probability) {
    int random_number=(int)(rand() % 100 + 1);
    int weight=(int)(100*probability);
    if(random_number<=weight) {
        return 1;
    }
    return 0;
}


bool gps_data_modifyGRID(epsilon_t eps, location_t* src, location_t* dest, int* n_locs, double flipone, double fliptwo){
    /*** to be replaced by bounding box partitioned into sub regions ***/
    int radius_meters=45;
    int regions_count=3;
    location_t regions[regions_count];
    regions[0].longitude=-118.482393;
    regions[0].latitude=34.015250;
    /*** Boelter Hall **/
    regions[1].longitude=-118.443084;
    regions[1].latitude=34.069439;
    /*** Wooden Center ***/
    regions[2].longitude=-118.445301;
    regions[2].latitude=34.071356;

    int destination_counter=0;
    for(int x=0;x<regions_count;x++) {
        double distance=GreatCircleDistanceInMeters(src->longitude,src->latitude,regions[x].longitude,regions[x].latitude);
        bool flipone=flip(flipone);
        bool fliptwo=flip(fliptwo);
        bool ismatched=0;
        if(distance<=radius_meters) {
            ismatched=1;
        }
        if(flipone) {
            //answer truthfully
        }
        else if(fliptwo) {
            ismatched=1;
        }
        else {
            ismatched=0;
        }

        if(ismatched) {
            dest[destination_counter].longitude = regions[x].longitude;
            dest[destination_counter].latitude = regions[x].latitude;
            destination_counter++;
        }
    }


    return 1;
}
