#include "setting_manager.h"
#include <stdlib.h>
#include <stdio.h>




setting_manager_t* setting_manager_new(setting_manager_t* sm, char* fname const){
    if (sm == NULL)
        return NULL;
    
    setting_manager_load_gps(sm, fname);
}

//bool setting_manager_delete(setting_manager_t*, char* fname);
////reading in and saving individual gps code

bool read_next_app_gps(FILE* file ,app_entry_t* dest){
    app_id_t m_id;
    gps_priv_t m_gps;
    //TODO ERROR CHECKING 
    //reading in id
    if ((fread(&m_id, sizeof(app_id_t), 1, file)) != 1)
        return 0;
    //reading in corresponding gps type
    if ((fread(&m_gps, sizeof(gps_priv_t),1, file) != 1))
        return 0;
    app_entry_new(dest, m_id, &m_gps);
    return 1;
}

//writes a single app_id_t to src
bool save_next_app_gps(FILE* file, app_entry_t* src){
    if (src == NULL || file == NULL)
        return 0;
    if (fwrite(&src->app_id, sizeof(app_id_t), 1, file) != 1)
        return 0;
    if (fwrite(&src->gps_setting, sizeof(gps_priv_t), 1, file) != 1)
        return 0;
    return 1;
}

//TODO do some error checking
bool setting_manager_load_gps(setting_manager_t* sm, char* fname const){
    FILE* gpsfile = fopen(fname, "r");
    //read in count
    int num_apps;
    if ((fread(&num_apps, sizeof(int), 1, gpsfile) != 1))
        return 0;

    sm->n_apps = num_apps;    
    //read in app_id and gps_settings
    int counter = 0;
    while (counter < num_apps){
        if (!read_next_app_gps(gpsfile, sm->apps + counter)){
            fclose(gpsfile);
            return 0;
        }
        counter++;
    }
    fclose(gpsfile);
    return 1;
}



bool setting_manager_save_gps(setting_manager_t* sm, char* fname const){
    FILE* gpsfile = fopen(fname, "w");
    if (fwrite(&sm->n_apps, sizeof(int), 1, gpsfile) != 1)
        return 0; 
    for (int i = 0; i < sm->n_apps;i++){
        if (!save_next_app_gps(gpsfile, sm->apps + i)){
            fclose(gpsfile);
            return 0;
        }
    }
    fclose(gpsfile);
    return 1;
}
