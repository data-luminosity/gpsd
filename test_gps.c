#include <string.h>
#include <syslog.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <libgen.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>

#include "gps.h"
#include "gpsd_config.h"
#include "gpsdclient.h"
#include "revision.h"


//log initial information
//  -app id
//  -epoch
//  -epsilon


//log information
//  -gps location
//  -time ellapsed since previous sent
//  -current count





int main(int argc, char** argv){
    unsigned int flags = WATCH_ENABLE;

}
