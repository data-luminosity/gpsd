#ifndef APP_ENTRY_H
#define APP_ENTRY_H
#include <stdbool.h>



#define GPS_EPOCH_DEFAULT 0
#define GPS_EPSILON_DEFAULT 0
#define GPS_TYPE_DEFAULT 0


/*
 *gps_priv is just settings of an app for gps
 *clear definition of a type, epoch, and epsilon
 */
typedef struct gps_priv{
    int type;
    int epoch;
    int epsilon;
} gps_priv_t;


gps_priv_t* gps_priv_new(gps_priv_t*, int t, int epo, int eps);
gps_priv_t* gps_priv_newDefault(gps_priv_t*);
gps_priv_t* gps_priv_copy(gps_priv_t* src, gps_priv_t* dest);



typedef int app_id_t;
typedef struct app_entry{
    app_id_t app_id;
    gps_priv_t gps_setting;
    bool is_modified; //modified flags to see if an app entry is modified
} app_entry_t;


app_entry_t* app_entry_new(app_entry_t* app, app_id_t, gps_priv_t*);
app_entry_t* app_entry_newDefault(app_entry_t* app, app_id_t);
app_entry_t* app_entry_copy(app_entry_t* src, app_entry_t* dest);
bool app_entry_set_gps_settings(app_entry_t* app, gps_priv_t *);
bool app_entry_equals(app_entry_t*, app_entry_t*);


#endif //APP_ENTRY_H
