#include "app_entry.h"
#include <stdio.h>



//GPS_PRIV
gps_priv_t* gps_priv_new(gps_priv_t* gps, int t, long epo, int eps){
    if (gps == NULL)
        return NULL;
    gps->type = t;
    gps->epoch = epo;
    gps->epsilon = eps;

    return gps;
}

gps_priv_t* gps_priv_newDefault(gps_priv_t* gps){
    if (gps == NULL)
        return NULL;
    gps->type = GPS_TYPE_DEFAULT;
    gps->epoch = GPS_EPOCH_DEFAULT;
    gps->epsilon = GPS_TYPE_DEFAULT;

    return gps;
}
gps_priv_t* gps_priv_copy(gps_priv_t* src, gps_priv_t* dest){
    if (src == NULL || dest == NULL)
        return NULL;
    dest->type = src->type;
    dest->epoch = src->epoch;
    dest->epsilon = src->epsilon;
    return src;
}

void gps_priv_dump(gps_priv_t* gps){
    printf("type: %d\t epoch: %ld\t epsilon:%d\t", gps->type, gps->epoch, gps->epsilon);
}

///APP ENTRY
app_entry_t* app_entry_new(app_entry_t* app, app_id_t id, gps_priv_t* gps){
    if (app == NULL)
        return NULL;
    app->app_id = id;
    app->is_modified = 0; 

    gps_priv_copy(gps,&(app->gps_setting));
    return app;
}

app_entry_t* app_entry_newDefault(app_entry_t* app, app_id_t id){
    if (app == NULL)
        return NULL;
    app->app_id = id;
    app->is_modified = false; 
    
    gps_priv_newDefault(&(app->gps_setting));
    return app;
}


bool app_entry_set_gps_settings(app_entry_t* app, gps_priv_t* new_gps){
    if (app == NULL || new_gps == NULL)
        return 0;
    gps_priv_copy(new_gps,&(app->gps_setting));
    return 1;
}

//equals when id are equal
bool app_entry_equals(app_entry_t* a , app_entry_t*b){
    return (a->app_id == b->app_id);
}

app_entry_t* app_entry_copy(app_entry_t* src, app_entry_t* dest){
    dest->app_id = src->app_id;
    gps_priv_copy(&src->gps_setting, &dest->gps_setting);
    
}

void app_entry_dump(app_entry_t* app){

    printf ("%d\tapp id:%d\t", app->app_id);
    gps_priv_t gps = app->gps_setting;
    gps_priv_dump(&(app->gps_setting));
    printf("\n");

}
