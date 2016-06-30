#include "app_entry.h"
#include "setting_manager.h"
#include "priv_handler.h"
#include "coordinate_calculations.h"
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    /*timeval_t prev_time;*/
    /*gettimeofday(&prev_time, NULL);*/
    /*printf("#####testing epoch diff#####\n");*/
    /*while(1){*/
        /*if (gps_epoch_allow_update(1000, &prev_time)){*/
            /*printf("allow!\n"); */
            /*gettimeofday(&prev_time, null);*/
        /*}*/
    /*}*/

    //manually creating app entries
    setting_manager_t sm;
    char* setting_name = "SETTINGS.txt";
    sm.n_apps = 100;
    
    //adding a bunch of dummy privacy entry
    long epo = 5000;
    double eps = 2;    
    
    gps_priv_t gps;
    app_entry_t app;
    for (int i = 0; i < 100; i++){
        gps_priv_new(&gps, 0, epo, eps);
        app_entry_new(&app, i, &gps);
        sm.apps[i] = app;
            

        epo += 100;
        eps += 1;
    }
    
    setting_manager_dump(&sm);    
    assert(setting_manager_save_gps(&sm, setting_name));
    /* 
    //loading up a new sm
    setting_manager_t sm2;
    assert(setting_manager_load_gps(&sm2, setting_name));

    setting_manager_dump(&sm2);   


    double distance=GreatCircleDistanceInMeters(-118.479024,34.012653,-118.482393,34.015250);
    printf("Distance=%f\n",distance);

    srand (time(NULL));
    //int random_number=(int)(100.0 * rand() / (RAND_MAX + 1.0)) + 1;
    int random_number=(int)(rand() % 100 + 1);
    printf("Random Number=%d\n",random_number);
    */
    /*
    printf("#####TESTING TIME DIFFERENCE\n###");
    timeval_t currtime, prev_time;
    gettimeofday(&prev_time, NULL);
    sleep(1);
    for (int i = 0;i <10; i++){
           
        gettimeofday(&currtime, NULL);
        long timediff = timediffval(&prev_time, &currtime);
        prev_time = currtime;
        printf("timediff #%d: %ld\n", i, timediff);
        sleep(i);
    }
    */


}
