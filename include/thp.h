#ifndef THP_H
#define THP_H

#include <stdarg.h>


typedef void (*thp_logcb)(const char *);

void	 thp_log_setcb(thp_logcb);

typedef void (*thp_punch_cb)(int, int, void *); /* event, socket, data */
struct thp_punch;

struct thp_punch        *thp_punch_start(const char *, const char *,
                                 const char *, thp_punch_cb, void *);
int                      thp_punch_stop(struct thp_punch *);

#endif