#include "app_entry.h"
#include "setting_manager.h"
#include "priv_handler.h"
#include <stdio.h>
#include <assert.h>


int main(){
    //creating some dummy app entries
    
    int eps1 = 100;
    int epo1 = 10;
   

    int eps2 = 100;
    int epo2 = 100;

    gps_priv_t gps1, gps2;
    gps_priv_new(&gps1, 0, epo1, eps1);
    gps_priv_new(&gps2, 0, epo2, eps2);
   
    app_entry_t app1,app2;
    app_entry_new(&app1, 0, &gps1);
    app_entry_new(&app2, 0, &gps2);

    //manually creating app entries
    setting_manager_t sm;
    char* setting_name = "SETTINGS.txt";
    sm.n_apps = 2;
    sm.apps[0] = app1;
    sm.apps[1] = app2;

    assert(setting_manager_save_gps( &sm,setting_name));
    
    //loading up a new sm
    setting_manager_t sm2;
    assert(setting_manager_load_gps(&sm2, setting_name));
    printf("setting_manger count: %d\n", sm2.n_apps);

     
    struct gps_fix_t gps, gps_priv;
    gps.longitude = 0;
    gps.latitude = 0;



    gps_data_modify(&(app1.gps_setting),&gps, &gps_priv);
    printf("longitude: %f\n" ,gps_priv.longitude);
    printf("latitude: %f\n" ,gps_priv.latitude);
}
