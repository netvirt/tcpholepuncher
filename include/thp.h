#ifndef THP_H
#define THP_H

#include <stdarg.h>

typedef void (*cb_log)(const char *);

void	 log_set_cb(cb_log);

#endif
