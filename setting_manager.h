#ifndef SETTING_MANAGER_H
#define SETTING_MANAGER_H
#include "app_entry.h"
#include <stdio.h>
#define SF_NAME "settings.txt"
#define N_APP_MAX 200

//structure holding current settings
//amount of apps is smalle enough to just search through it
typedef struct setting_manager{
    app_entry_t apps[N_APP_MAX];
    int n_apps;
    //TODO possibly a bitmap
} setting_manager_t;

setting_manager_t* setting_manager_new(setting_manager_t*, char* const);
//bool setting_manager_delete(setting_manager_t*);

bool setting_manager_load_gps(setting_manager_t*, char* const);
bool setting_manager_save_gps(setting_manager_t*, char* const);

bool read_next_app_gps(FILE* file ,app_entry_t* dest);
bool save_next_app_gps(FILE* file, app_entry_t* src);

app_id_t setting_manager_get_app_entry(setting_manager_t* , app_id_t, app_entry_t* dest);
app_id_t* setting_manager_set_app_entry(setting_manager_t* , app_entry_t* src, app_entry_t* dest);

const gps_priv_t DEFAULT_GPS_PRIV_SETTINGS = {
.type = GPS_TYPE_DEFAULT,
.epoch = GPS_EPOCH_DEFAULT,
.epsilon = GPS_EPSILON_DEFAULT
};


#endif
