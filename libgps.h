/* libgps.h -- prototypes for internals of the libgps library */
/*
 * This file is Copyright (c) 2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */
#ifndef _GPSD_LIBGPS_H_
#define _GPSD_LIBGPS_H_

/* values to poke in the gps_fd member if we get it via something special */
#define SHM_PSEUDO_FD	-1
#define DBUS_PSEUDO_FD	-2

#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int gps_sock_open(const char *, const char *,
		      struct gps_data_t *);
extern int gps_sock_close(struct gps_data_t *);
extern int gps_sock_send(struct gps_data_t *, const char *);
extern int gps_sock_read(struct gps_data_t *);
extern bool gps_sock_waiting(const struct gps_data_t *, int);
extern int gps_sock_stream(struct gps_data_t *, unsigned int, void *);

//SOCKET INTERFACE FOR PASSING IN ID//
extern int gps_sock_pass_appid(struct gps_data_t *, int, void *);//TODO again, int is of type app_id_t

extern const char *gps_sock_data(const struct gps_data_t *);
extern int gps_sock_mainloop(struct gps_data_t *, int,
			      void (*)(struct gps_data_t *));
extern int gps_shm_open(struct gps_data_t *);
extern void gps_shm_close(struct gps_data_t *);
extern bool gps_shm_waiting(const struct gps_data_t *, int);
extern int gps_shm_read(struct gps_data_t *);
extern int gps_shm_mainloop(struct gps_data_t *, int,
			      void (*)(struct gps_data_t *));

extern int gps_dbus_open(struct gps_data_t *);
extern int gps_dbus_mainloop(struct gps_data_t *, int,
			      void (*)(struct gps_data_t *));

extern int json_ais_read(const char *, char *, size_t, struct ais_t *,
			 const char **);
#ifdef __cplusplus
}
#endif

#define PRIVATE(gpsdata) ((struct privdata_t *)(gpsdata)->privdata)

#endif /* _GPSD_LIBGPS_H_ */
